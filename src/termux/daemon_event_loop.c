#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

#include "daemon.h"
#include "protocol.h"

static void update_pty_size(const t_mux_state *state, const int master_fd) {
  if (state->count == 0)
    return;
  struct winsize min_ws = state->clients[0]->ws;
  for (int i = 1; i < state->count; i++) {
    if (state->clients[i]->ws.ws_col > 0 &&
        state->clients[i]->ws.ws_col < min_ws.ws_col)
      min_ws.ws_col = state->clients[i]->ws.ws_col;
    if (state->clients[i]->ws.ws_row > 0 &&
        state->clients[i]->ws.ws_row < min_ws.ws_row)
      min_ws.ws_row = state->clients[i]->ws.ws_row;
  }
  ioctl(master_fd, TIOCSWINSZ, &min_ws);
}

static void queue_pty_write(t_mux_state *state, const void *data, size_t len) {
  if (state->pty_write_pos + len > IO_BUF_SIZE)
    return;
  memcpy(state->pty_write_buf + state->pty_write_pos, data, len);
  state->pty_write_pos += len;
}

static void disconnect_client(t_mux_state *state, const int index,
                              const int master_fd) {
  close(state->clients[index]->fd);
  free(state->clients[index]);

  for (int i = index; i < state->count - 1; i++) {
    state->clients[i] = state->clients[i + 1];
    state->poll_fds[i + 2] = state->poll_fds[i + 3];
  }
  state->count--;
  update_pty_size(state, master_fd);
}

static void queue_send(t_client *client, const e_msg_type type,
                       const void *data, uint32_t len) {
  const size_t total_len = sizeof(t_msg_header) + len;
  if (client->write_pos + total_len > IO_BUF_SIZE)
    return;

  const t_msg_header hdr = {.type = type, .len = htonl(len)};
  memcpy(client->write_buf + client->write_pos, &hdr, sizeof(hdr));
  if (len > 0 && data) {
    memcpy(client->write_buf + client->write_pos + sizeof(hdr), data, len);
  }
  client->write_pos += total_len;
}

void daemon_event_loop(const int server_fd, const int master_fd) {
  t_mux_state *state = calloc(1, sizeof(t_mux_state));
  if (!state) {
    perror("daemon: calloc failed");
    return;
  }

  state->poll_fds[0].fd = server_fd;
  state->poll_fds[1].fd = master_fd;

  while (true) {
    state->poll_fds[0].events = POLLIN;
    state->poll_fds[1].events = POLLIN;
    if (state->pty_write_pos > 0)
      state->poll_fds[1].events |= POLLOUT;

    for (int i = 0; i < state->count; i++) {
      state->poll_fds[i + 2].events = POLLIN;
      if (state->clients[i]->write_pos > 0)
        state->poll_fds[i + 2].events |= POLLOUT;
    }

    if (poll(state->poll_fds, 2 + state->count, -1) < 0) {
      if (errno == EINTR)
        continue;
      break;
    }

    if (state->poll_fds[1].revents & POLLOUT) {
      const ssize_t sent =
          write(master_fd, state->pty_write_buf, state->pty_write_pos);
      if (sent > 0) {
        state->pty_write_pos -= sent;
        if (state->pty_write_pos > 0)
          memmove(state->pty_write_buf, state->pty_write_buf + sent,
                  state->pty_write_pos);
      }
    }

    if (state->poll_fds[0].revents & POLLIN) {
      const int client_fd = accept(server_fd, NULL, NULL);
      if (client_fd >= 0) {
        if (state->count < SOMAXCONN) {
          t_client *new_client = calloc(1, sizeof(t_client));
          if (!new_client) {
            close(client_fd);
          } else {
            fcntl(client_fd, F_SETFD, FD_CLOEXEC);
            fcntl(client_fd, F_SETFL, O_NONBLOCK);

            new_client->fd = client_fd;
            state->clients[state->count] = new_client;
            state->poll_fds[state->count + 2].fd = client_fd;
            state->count++;
          }
        } else {
          close(client_fd);
        }
      }
    }

    if (state->poll_fds[1].revents & (POLLIN | POLLHUP | POLLERR)) {
      uint8_t pty_buf[4096];
      ssize_t n = read(master_fd, pty_buf, sizeof(pty_buf));
      if (n > 0) {
        for (int i = 0; i < state->count; i++)
          queue_send(state->clients[i], MSG_DATA, pty_buf, n);
      } else if (n == 0 || (n == -1 && errno == EIO)) {
        break;
      }
    }

    for (int i = 0; i < state->count;) {
      t_client *c = state->clients[i];
      bool disconnected = false;

      if (state->poll_fds[i + 2].revents & POLLOUT) {
        const ssize_t sent = write(c->fd, c->write_buf, c->write_pos);
        if (sent > 0) {
          c->write_pos -= sent;
          if (c->write_pos > 0)
            memmove(c->write_buf, c->write_buf + sent, c->write_pos);
        } else if (sent < 0 && errno != EAGAIN) {
          disconnected = true;
        }
      }

      if (!disconnected && (state->poll_fds[i + 2].revents & POLLIN)) {
        const ssize_t n =
            read(c->fd, c->read_buf + c->read_pos, IO_BUF_SIZE - c->read_pos);
        if (n > 0) {
          c->read_pos += n;
          size_t parse_offset = 0;

          while (c->read_pos - parse_offset >= sizeof(t_msg_header)) {
            t_msg_header *hdr = (t_msg_header *)(c->read_buf + parse_offset);
            uint32_t payload_len = ntohl(hdr->len);

            if (payload_len > IO_BUF_SIZE - sizeof(t_msg_header)) {
              disconnected = true;
              break;
            }

            if (c->read_pos - parse_offset < sizeof(t_msg_header) + payload_len)
              break;

            uint8_t *payload =
                c->read_buf + parse_offset + sizeof(t_msg_header);

            if (hdr->type == MSG_DATA) {
              queue_pty_write(state, payload, payload_len);
            } else if (hdr->type == MSG_WINCH &&
                       payload_len == sizeof(struct winsize)) {
              c->ws = *(struct winsize *)payload;
              update_pty_size(state, master_fd);
            }
            parse_offset += sizeof(t_msg_header) + payload_len;
          }

          if (parse_offset > 0 && !disconnected) {
            c->read_pos -= parse_offset;
            if (c->read_pos > 0)
              memmove(c->read_buf, c->read_buf + parse_offset, c->read_pos);
          }
        } else if (n == 0 || (n < 0 && errno != EAGAIN)) {
          disconnected = true;
        }
      }

      if (disconnected) {
        disconnect_client(state, i, master_fd);
      } else {
        i++;
      }
    }
  }

  for (int i = 0; i < state->count; i++) {
    close(state->clients[i]->fd);
    free(state->clients[i]);
  }
  free(state);
}