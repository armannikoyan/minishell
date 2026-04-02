#ifdef __linux__
#define _XOPEN_SOURCE 600
#endif

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <unistd.h>

#include "daemon.h"
#include "minishell.h"
#include "mux.h"

int spawn_pty_session(t_mux_state *state, const int server_fd) {
  const int master_fd = posix_openpt(O_RDWR | O_NOCTTY);
  if (master_fd < 0)
    return -1;

  const int flags = fcntl(master_fd, F_GETFD);
  if (flags != -1)
    fcntl(master_fd, F_SETFD, flags | FD_CLOEXEC);

  if (grantpt(master_fd) < 0 || unlockpt(master_fd) < 0) {
    close(master_fd);
    return -1;
  }

  char *slave_name = ptsname(master_fd);
  if (!slave_name) {
    close(master_fd);
    return -1;
  }

  const pid_t pid = fork();
  if (pid < 0) {
    close(master_fd);
    return -1;
  }

  if (pid == 0) {
    close(master_fd);
    close(server_fd);

    for (int i = 0; i < state->window_count; i++) {
      close(state->windows[i].fd);
    }

    for (int i = 0; i < state->client_count; i++) {
      close(state->clients[i]->fd);
      free(state->clients[i]);
    }

    char **envp_copy = state->envp;

    free(state);

    setsid();

    const int slave_fd = open(slave_name, O_RDWR);
    if (slave_fd < 0)
      exit(EXIT_FAILURE);

    dup2(slave_fd, STDIN_FILENO);
    dup2(slave_fd, STDOUT_FILENO);
    dup2(slave_fd, STDERR_FILENO);

    const long max_fd = sysconf(_SC_OPEN_MAX);
    for (int i = 3; i < max_fd; i++) {
      close(i);
    }

    run_interactive_shell(envp_copy);
    exit(EXIT_SUCCESS);
  }

  fcntl(master_fd, F_SETFL, O_NONBLOCK);
  return master_fd;
}

int run_daemon(char **envp, const char *session_name) {
  int server_fd;
  struct sockaddr_un addr;
  char socket_path[1024];

  FORMAT_SOCKET_PATH(socket_path, sizeof(socket_path), session_name);
  signal(SIGPIPE, SIG_IGN);

  if ((server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    return EXIT_FAILURE;
  fcntl(server_fd, F_SETFD, FD_CLOEXEC);

  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path) - 1);

  if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
    if (errno == EADDRINUSE) {
      const int test_fd = socket(AF_UNIX, SOCK_STREAM, 0);
      if (connect(test_fd, (struct sockaddr *)&addr, sizeof(addr)) == 0) {
        close(test_fd);
        close(server_fd);
        return EXIT_SUCCESS;
      }
      close(test_fd);
      unlink(socket_path);
      if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        close(server_fd);
        return EXIT_FAILURE;
      }
    } else {
      close(server_fd);
      return EXIT_FAILURE;
    }
  }

  if (listen(server_fd, 5) == -1) {
    close(server_fd);
    unlink(socket_path);
    return EXIT_FAILURE;
  }

  chmod(socket_path, 0600);
  fcntl(server_fd, F_SETFL, O_NONBLOCK);

  daemon_event_loop(server_fd, envp);

  close(server_fd);
  unlink(socket_path);

  return EXIT_SUCCESS;
}