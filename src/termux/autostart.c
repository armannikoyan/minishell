#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <unistd.h>

#include "mux.h"

static bool is_daemon_alive(const char *session_name) {
  const int sock = socket(AF_UNIX, SOCK_STREAM, 0);
  if (sock == -1)
    return false;

  struct sockaddr_un addr;
  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  FORMAT_SOCKET_PATH(addr.sun_path, sizeof(addr.sun_path), session_name);

  if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == 0) {
    close(sock);
    return true;
  } else {
    if (errno == EACCES) {
      fprintf(stderr,
              "minishell: permission denied to connect to session '%s'\n",
              session_name);
      close(sock);
      exit(EXIT_FAILURE);
    }
  }

  close(sock);
  return false;
}

int auto_attach_or_spawn(char **envp, const char *session_name) {
  if (is_daemon_alive(session_name)) {
    return run_client(session_name);
  }

  const pid_t pid = fork();
  if (pid < 0) {
    perror("minishell: fork failed");
    return EXIT_FAILURE;
  }

  if (pid == 0) {
    setsid();

    pid_t pid2 = fork();
    if (pid2 < 0)
      exit(EXIT_FAILURE);

    if (pid2 > 0) {
      exit(EXIT_SUCCESS);
    }

    const int devnull = open("/dev/null", O_RDONLY);
    if (devnull != -1) {
      dup2(devnull, STDIN_FILENO);
      close(devnull);
    }

    const int log_fd =
        open("/tmp/minishell_daemon.log", O_WRONLY | O_CREAT | O_APPEND, 0600);
    if (log_fd != -1) {
      dup2(log_fd, STDOUT_FILENO);
      dup2(log_fd, STDERR_FILENO);
      close(log_fd);
    }

    exit(run_daemon(envp, session_name));
  }

  waitpid(pid, NULL, 0);

  int retries = 50;
  while (!is_daemon_alive(session_name) && retries-- > 0) {
    usleep(10000);
  }

  if (retries <= 0) {
    fprintf(
        stderr,
        "minishell: failed to start or connect to daemon for session '%s'\n",
        session_name);
    return EXIT_FAILURE;
  }

  return run_client(session_name);
}