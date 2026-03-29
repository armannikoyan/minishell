#ifndef DAEMON_H
#define DAEMON_H

#ifdef __APPLE__
#include <sys/ttycom.h>
#endif

#ifdef __linux__
#include <linux/tty.h>
#endif

#include <poll.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <termios.h>

#define BUFFER_SIZE 4096
#define IO_BUF_SIZE 8192

typedef struct {
  struct pollfd fds[SOMAXCONN + 2];
  int count;
} t_poll_manager;

typedef struct {
  int fd;

  uint8_t read_buf[IO_BUF_SIZE];
  size_t read_pos;

  uint8_t write_buf[IO_BUF_SIZE];
  size_t write_pos;

  struct winsize ws;
} t_client;

typedef struct {
  struct pollfd poll_fds[SOMAXCONN + 2];
  t_client *clients[SOMAXCONN];
  int count;

  uint8_t pty_write_buf[IO_BUF_SIZE];
  size_t pty_write_pos;
} t_mux_state;

void daemon_event_loop(int server_fd, int master_fd);

#endif
