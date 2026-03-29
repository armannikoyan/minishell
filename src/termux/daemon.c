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

int spawn_pty_session(void) {
  const int master_fd = posix_openpt(O_RDWR | O_NOCTTY);
  if (master_fd < 0)
    return -1;

  const int flags = fcntl(master_fd, F_GETFD);
  if (flags != -1) {
    fcntl(master_fd, F_SETFD, flags | FD_CLOEXEC);
  }

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

    setsid();

    const int slave_fd = open(slave_name, O_RDWR);
    if (slave_fd < 0)
      exit(EXIT_FAILURE);

    dup2(slave_fd, STDIN_FILENO);
    dup2(slave_fd, STDOUT_FILENO);
    dup2(slave_fd, STDERR_FILENO);

    if (slave_fd > STDERR_FILENO) {
      close(slave_fd);
    }

    return 0;
  }

  return master_fd;
}

int run_daemon(char **envp, const char *session_name) {
  int server_fd;
  struct sockaddr_un addr;
  char socket_path[1024];

  FORMAT_SOCKET_PATH(socket_path, sizeof(socket_path), session_name);

  signal(SIGPIPE, SIG_IGN);

  if ((server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
    perror("daemon: socket");
    return EXIT_FAILURE;
  }

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
        printf("daemon: another instance won the race. Exiting.\n");
        return EXIT_SUCCESS;
      }
      close(test_fd);

      unlink(socket_path);
      if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("daemon: bind retry");
        close(server_fd);
        return EXIT_FAILURE;
      }
    } else {
      perror("daemon: bind");
      close(server_fd);
      return EXIT_FAILURE;
    }
  }

  if (listen(server_fd, 5) == -1) {
    perror("daemon: listen");
    close(server_fd);
    unlink(socket_path);
    return EXIT_FAILURE;
  }

  chmod(socket_path, 0600);

  const int master_fd = spawn_pty_session();
  if (master_fd == -1) {
    perror("daemon: pty spawn failed");
    close(server_fd);
    unlink(socket_path);
    return EXIT_FAILURE;
  }
  if (master_fd == 0) {
    close(server_fd);
    run_interactive_shell(envp);
    exit(EXIT_SUCCESS);
  }

  fcntl(server_fd, F_SETFL, O_NONBLOCK);
  fcntl(master_fd, F_SETFL, O_NONBLOCK);

  printf("daemon: started successfully for session %s\n", session_name);

  daemon_event_loop(server_fd, master_fd);

  return EXIT_SUCCESS;
}