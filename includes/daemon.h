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

#include "protocol.h"

#define MAX_WINDOWS 10
#define BUFFER_SIZE 4096
#define IO_BUF_SIZE 8192
#define HISTORY_SIZE 8000

typedef struct {
  int fd;
  uint8_t write_buf[IO_BUF_SIZE];
  size_t write_pos;

  uint8_t history[HISTORY_SIZE];
  size_t hist_head;
  size_t hist_len;
} t_window;

typedef struct {
  int fd;
  uint8_t read_buf[IO_BUF_SIZE];
  size_t read_pos;
  uint8_t write_buf[IO_BUF_SIZE];
  size_t write_pos;
  struct winsize ws;

  int active_window;
} t_client;

typedef struct {
  struct pollfd poll_fds[SOMAXCONN + MAX_WINDOWS + 2];
  t_client *clients[SOMAXCONN];
  int client_count;

  t_window windows[MAX_WINDOWS];
  int window_count;

  char **envp;
} t_mux_state;

void daemon_event_loop(int server_fd, char **envp);
int spawn_pty_session(char **envp);

#endif