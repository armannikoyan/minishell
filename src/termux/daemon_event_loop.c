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

static bool add_window(t_mux_state *state, const int server_fd) {
  if (state->window_count >= MAX_WINDOWS)
    return false;

  const int fd = spawn_pty_session(state, server_fd);
  if (fd > 0) {
    state->windows[state->window_count].fd = fd;
    state->windows[state->window_count].write_pos = 0;

    state->windows[state->window_count].hist_head = 0;
    state->windows[state->window_count].hist_len = 0;
    memset(state->windows[state->window_count].history, 0, HISTORY_SIZE);

    state->window_count++;
    return true;
  }
  return false;
}

static void update_pty_size(const t_mux_state *state, const int win_index) {
  if (win_index < 0 || win_index >= state->window_count ||
      state->client_count == 0)
    return;

  struct winsize min_ws = {0, 0, 0, 0};
  bool found = false;

  for (int i = 0; i < state->client_count; i++) {
    if (state->clients[i]->active_window == win_index) {
      if (!found || (state->clients[i]->ws.ws_row < min_ws.ws_row &&
                     state->clients[i]->ws.ws_row > 0)) {
        min_ws = state->clients[i]->ws;
        found = true;
      }
    }
  }

  if (found && min_ws.ws_row > 1) {
    min_ws.ws_row -= 1;
    ioctl(state->windows[win_index].fd, TIOCSWINSZ, &min_ws);
  }
}

static void queue_pty_write(t_window *win, const void *data, size_t len) {
  if (win->write_pos + len > IO_BUF_SIZE)
    return;
  memcpy(win->write_buf + win->write_pos, data, len);
  win->write_pos += len;
}

static void disconnect_client(t_mux_state *state, const int index) {
  close(state->clients[index]->fd);
  free(state->clients[index]);
  for (int i = index; i < state->client_count - 1; i++) {
    state->clients[i] = state->clients[i + 1];
  }
  state->client_count--;
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

static void append_history(t_window *win, const uint8_t *data, size_t len) {
  for (size_t i = 0; i < len; i++) {
    win->history[win->hist_head] = data[i];
    win->hist_head = (win->hist_head + 1) % HISTORY_SIZE;
    if (win->hist_len < HISTORY_SIZE) {
      win->hist_len++;
    }
  }
}

static void send_window_history(t_client *client, const t_window *win) {
  if (win->hist_len == 0)
    return;

  uint8_t buf[HISTORY_SIZE];
  size_t start = (win->hist_len == HISTORY_SIZE) ? win->hist_head : 0;

  for (size_t i = 0; i < win->hist_len; i++) {
    buf[i] = win->history[(start + i) % HISTORY_SIZE];
  }
  queue_send(client, MSG_DATA, buf, win->hist_len);
}

static void send_clear_and_anchor(t_client *client) {
  queue_send(client, MSG_DATA, "\033[0m\033[2J\033[H", 11);
}

static void draw_status_bar(const t_mux_state *state, t_client *client) {
  if (client->ws.ws_row <= 1)
    return;

  char buf[2048];
  size_t len = 0;

  len += snprintf(buf + len, sizeof(buf) - len, "\0337\033[1;%dr\033[%d;1H",
                  client->ws.ws_row - 1, client->ws.ws_row);

  for (int i = 0; i < state->window_count && len < sizeof(buf) - 64; i++) {
    if (i == client->active_window) {
      len += snprintf(buf + len, sizeof(buf) - len,
                      "\033[42;30m [ %d ] \033[0m", i);
    } else {
      len +=
          snprintf(buf + len, sizeof(buf) - len, "\033[43;30m  %d  \033[0m", i);
    }
  }

  len += snprintf(buf + len, sizeof(buf) - len, "\033[K\033[0m\0338");

  queue_send(client, MSG_DATA, buf, len);
}

static void redraw_all_status_bars(const t_mux_state *state) {
  for (int i = 0; i < state->client_count; i++) {
    draw_status_bar(state, state->clients[i]);
  }
}

// ИСПРАВЛЕНИЕ #2: Логика удаления окна и очистки артефактов
static void remove_window(t_mux_state *state, const int index) {
  close(state->windows[index].fd);
  for (int i = index; i < state->window_count - 1; i++) {
    state->windows[i] = state->windows[i + 1];
  }
  state->window_count--;

  if (state->window_count == 0)
    return; // Демон всё равно завершится

  for (int c = 0; c < state->client_count; c++) {
    bool focus_changed = false;

    if (state->clients[c]->active_window == index) {
      // Активное окно умерло. Перекидываем на предыдущее (или на 0)
      state->clients[c]->active_window = (index > 0) ? index - 1 : 0;
      focus_changed = true;
    } else if (state->clients[c]->active_window > index) {
      // Индекс активного окна сместился влево
      state->clients[c]->active_window--;
    }

    if (focus_changed) {
      int new_win = state->clients[c]->active_window;
      update_pty_size(state, new_win);

      // Очищаем экран клиента от старой сессии
      send_clear_and_anchor(state->clients[c]);
      send_window_history(state->clients[c], &state->windows[new_win]);
      draw_status_bar(state, state->clients[c]);

      // Отправляем Ctrl+L в новую активную сессию, чтобы readline перерисовал
      // строку ввода
      // char ctrl_l = 0x0C;
      // queue_pty_write(&state->windows[new_win], &ctrl_l, 1);
    } else {
      // Просто обновляем цифры в статус-баре
      draw_status_bar(state, state->clients[c]);
    }
  }
}

void daemon_event_loop(const int server_fd, char **envp) {
  t_mux_state *state = calloc(1, sizeof(t_mux_state));
  if (!state)
    return;
  state->envp = envp;

  add_window(state, server_fd);

  bool daemon_running = true;

  while (daemon_running && state->window_count > 0) {
    int nfds = 0;

    state->poll_fds[nfds].fd = server_fd;
    state->poll_fds[nfds].events = POLLIN;
    state->poll_fds[nfds].revents = 0;
    const int server_poll_idx = nfds++;

    const int win_poll_offset = nfds;
    for (int i = 0; i < state->window_count; i++) {
      state->poll_fds[nfds].fd = state->windows[i].fd;
      state->poll_fds[nfds].events = POLLIN;
      if (state->windows[i].write_pos > 0)
        state->poll_fds[nfds].events |= POLLOUT;
      state->poll_fds[nfds].revents = 0;
      nfds++;
    }

    const int client_poll_offset = nfds;
    for (int i = 0; i < state->client_count; i++) {
      state->poll_fds[nfds].fd = state->clients[i]->fd;
      state->poll_fds[nfds].events = POLLIN;
      if (state->clients[i]->write_pos > 0)
        state->poll_fds[nfds].events |= POLLOUT;
      state->poll_fds[nfds].revents = 0;
      nfds++;
    }

    if (poll(state->poll_fds, nfds, -1) < 0) {
      if (errno == EINTR)
        continue;
      break;
    }

    if (state->poll_fds[server_poll_idx].revents & POLLIN) {
      const int client_fd = accept(server_fd, NULL, NULL);
      if (client_fd >= 0) {
        if (state->client_count < SOMAXCONN) {
          t_client *new_client = calloc(1, sizeof(t_client));
          if (new_client) {
            fcntl(client_fd, F_SETFD, FD_CLOEXEC);
            fcntl(client_fd, F_SETFL, O_NONBLOCK);
            new_client->fd = client_fd;
            new_client->active_window = 0;
            state->clients[state->client_count++] = new_client;
            queue_send(new_client, MSG_DATA, "\033[0m\033[2J\033[H", 11);
            update_pty_size(state, 0);
            send_window_history(new_client, &state->windows[0]);
            draw_status_bar(state, new_client);
          } else {
            close(client_fd);
          }
        } else {
          close(client_fd);
        }
      }
    }

    for (int i = 0; i < state->window_count;) {
      bool win_closed = false;
      int p_idx = win_poll_offset + i;

      if (state->poll_fds[p_idx].revents & POLLOUT) {
        ssize_t sent = write(state->windows[i].fd, state->windows[i].write_buf,
                             state->windows[i].write_pos);
        if (sent > 0) {
          state->windows[i].write_pos -= sent;
          if (state->windows[i].write_pos > 0)
            memmove(state->windows[i].write_buf,
                    state->windows[i].write_buf + sent,
                    state->windows[i].write_pos);
        }
      }

      if (state->poll_fds[p_idx].revents & (POLLIN | POLLHUP | POLLERR)) {
        uint8_t pty_buf[4096];
        ssize_t n = read(state->windows[i].fd, pty_buf, sizeof(pty_buf));
        if (n > 0) {
          append_history(&state->windows[i], pty_buf, n);

          for (int c = 0; c < state->client_count; c++) {
            if (state->clients[c]->active_window == i) {
              queue_send(state->clients[c], MSG_DATA, pty_buf, n);
              draw_status_bar(state, state->clients[c]);
            }
          }
        } else if (n == 0 || (n == -1 && errno == EIO)) {
          win_closed = true;
        }
      }

      if (win_closed) {
        remove_window(state, i);
      } else {
        i++;
      }
    }

    for (int i = 0; i < state->client_count;) {
      t_client *c = state->clients[i];
      bool disconnected = false;
      int p_idx = client_poll_offset + i;

      if (state->poll_fds[p_idx].revents & POLLOUT) {
        const ssize_t sent = write(c->fd, c->write_buf, c->write_pos);
        if (sent > 0) {
          c->write_pos -= sent;
          if (c->write_pos > 0)
            memmove(c->write_buf, c->write_buf + sent, c->write_pos);
        } else if (sent < 0 && errno != EAGAIN) {
          disconnected = true;
        }
      }

      if (!disconnected && (state->poll_fds[p_idx].revents & POLLIN)) {
        const ssize_t n =
            read(c->fd, c->read_buf + c->read_pos, IO_BUF_SIZE - c->read_pos);
        if (n > 0) {
          c->read_pos += n;
          size_t parse_offset = 0;

          while (c->read_pos - parse_offset >= sizeof(t_msg_header)) {
            t_msg_header *hdr = (t_msg_header *)(c->read_buf + parse_offset);
            const uint32_t payload_len = ntohl(hdr->len);

            if (payload_len > IO_BUF_SIZE - sizeof(t_msg_header)) {
              disconnected = true;
              break;
            }
            if (c->read_pos - parse_offset < sizeof(t_msg_header) + payload_len)
              break;

            uint8_t *payload =
                c->read_buf + parse_offset + sizeof(t_msg_header);

            if (hdr->type == MSG_DATA) {
              if (c->active_window < state->window_count)
                queue_pty_write(&state->windows[c->active_window], payload,
                                payload_len);

            } else if (hdr->type == MSG_CREATE_WIN) {
              if (add_window(state, server_fd)) {
                c->active_window = state->window_count - 1;
                update_pty_size(state, c->active_window);
                send_clear_and_anchor(c);
                send_clear_and_anchor(c);
                redraw_all_status_bars(state);
              }
            } else if (hdr->type == MSG_SWITCH_WIN &&
                       payload_len == sizeof(int)) {
              int target_idx = ntohl(*(int *)payload);
              if (target_idx < state->window_count &&
                  target_idx != c->active_window) {
                c->active_window = target_idx;
                update_pty_size(state, target_idx);
                send_clear_and_anchor(c);
                send_window_history(c, &state->windows[c->active_window]);
                draw_status_bar(state, c);
              }
            } else if (hdr->type == MSG_REDRAW_UI) {
              send_clear_and_anchor(c);
              send_window_history(c, &state->windows[c->active_window]);
              draw_status_bar(state, c);

            } else if (hdr->type == MSG_SHUTDOWN) {
              daemon_running = false;
              break;

            } else if (hdr->type == MSG_WINCH &&
                       payload_len == sizeof(struct winsize)) {
              c->ws = *(struct winsize *)payload;
              update_pty_size(state, c->active_window);
              draw_status_bar(state, c);
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

      if (disconnected)
        disconnect_client(state, i);
      else
        i++;
    }
  }
  for (int i = 0; i < state->window_count; i++) {
    close(state->windows[i].fd);
  }

  for (int i = 0; i < state->client_count; i++) {
    close(state->clients[i]->fd);
    free(state->clients[i]);
  }

  free(state);
}