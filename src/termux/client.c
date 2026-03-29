#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <termios.h>
#include <unistd.h>

#include "mux.h"
#include "protocol.h"

static struct termios g_orig_termios;
static volatile sig_atomic_t g_winch_flag = 1;

static void sigwinch_handler(int sig __attribute__((unused))) {
  g_winch_flag = 1;
}

static void restore_terminal(void) {
  tcsetattr(STDIN_FILENO, TCSANOW, &g_orig_termios);
}

static void enable_raw_mode(void) {
  tcgetattr(STDIN_FILENO, &g_orig_termios);
  atexit(restore_terminal);

  struct termios raw = g_orig_termios;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

  tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

static void send_frame(const int sock, const e_msg_type type, const void *data,
                       uint32_t len) {
  const t_msg_header hdr = {.type = type, .len = htonl(len)};
  write(sock, &hdr, sizeof(hdr));
  if (len > 0 && data)
    write(sock, data, len);
}

int run_client(const char *session_name) {
  int sock;
  struct sockaddr_un addr;
  char socket_path[1024];

  FORMAT_SOCKET_PATH(socket_path, sizeof(socket_path), session_name);

  if ((sock = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    return EXIT_FAILURE;

  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path) - 1);

  if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
    close(sock);
    return EXIT_FAILURE;
  }

  if (isatty(STDIN_FILENO)) {
    enable_raw_mode();
    struct sigaction sa = {0};
    sa.sa_handler = sigwinch_handler;
    sigaction(SIGWINCH, &sa, NULL);
  }

  struct pollfd fds[2];
  fds[0].fd = STDIN_FILENO;
  fds[0].events = POLLIN;
  fds[1].fd = sock;
  fds[1].events = POLLIN;

  uint8_t read_buf[8192];
  size_t read_pos = 0;

  while (true) {
    if (g_winch_flag) {
      g_winch_flag = 0;
      struct winsize ws;
      if (ioctl(STDIN_FILENO, TIOCGWINSZ, &ws) != -1) {
        send_frame(sock, MSG_WINCH, &ws, sizeof(ws));
      }
    }

    if (poll(fds, 2, -1) < 0) {
      if (errno == EINTR)
        continue;
      break;
    }

    if (fds[0].revents & POLLIN) {
      uint8_t buf[1024];
      ssize_t n = read(STDIN_FILENO, buf, sizeof(buf));
      if (n > 0)
        send_frame(sock, MSG_DATA, buf, n);
      else if (n <= 0)
        break;
    }

    if (fds[1].revents & POLLIN) {
      ssize_t n = read(sock, read_buf + read_pos, sizeof(read_buf) - read_pos);
      if (n > 0) {
        read_pos += n;
        size_t parse_offset = 0;

        while (read_pos - parse_offset >= sizeof(t_msg_header)) {
          t_msg_header *hdr = (t_msg_header *)(read_buf + parse_offset);
          const uint32_t payload_len = ntohl(hdr->len);

          if (payload_len > sizeof(read_buf) - sizeof(t_msg_header))
            break;
          if (read_pos - parse_offset < sizeof(t_msg_header) + payload_len)
            break;

          if (hdr->type == MSG_DATA) {
            write(STDOUT_FILENO, read_buf + parse_offset + sizeof(t_msg_header),
                  payload_len);
          }
          parse_offset += sizeof(t_msg_header) + payload_len;
        }

        if (parse_offset > 0) {
          read_pos -= parse_offset;
          if (read_pos > 0)
            memmove(read_buf, read_buf + parse_offset, read_pos);
        }
      } else {
        break;
      }
    }
  }

  close(sock);
  return EXIT_SUCCESS;
}