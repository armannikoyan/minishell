#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <unistd.h>

#include "mux.h"
#include "protocol.h"

#include <dirent.h>

// Возвращает: 1 (жив), 0 (мертв), -1 (нет доступа / чужой сокет)
static int check_daemon_status(const char *session_name) {
  const int sock = socket(AF_UNIX, SOCK_STREAM, 0);
  if (sock == -1)
    return 0;

  struct sockaddr_un addr;
  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  FORMAT_SOCKET_PATH(addr.sun_path, sizeof(addr.sun_path), session_name);

  if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == 0) {
    close(sock);
    return 1;
  } else {
    if (errno == EACCES || errno == EPERM) {
      close(sock);
      return -1;
    }
  }

  close(sock);
  return 0;
}

static bool is_daemon_alive(const char *session_name) {
  int status = check_daemon_status(session_name);
  if (status == -1) {
    fprintf(stderr, "minishell: permission denied to connect to session '%s'\n",
            session_name);
    exit(EXIT_FAILURE);
  }
  return status == 1;
}

int list_sessions(void) {
  DIR *dir = opendir("/tmp");
  if (!dir) {
    perror("minishell: cannot open /tmp");
    return EXIT_FAILURE;
  }

  struct dirent *entry;
  bool found = false;

  printf("Active minishell sessions:\n");
  printf("--------------------------------------------------\n");

  while ((entry = readdir(dir)) != NULL) {
    if (strncmp(entry->d_name, "minishell_mux_", 14) == 0) {
      const char *ext = strstr(entry->d_name, MUX_SOCKET_EXT);
      if (ext) {
        char session_name[256] = {0};
        size_t name_len = ext - (entry->d_name + 14);

        if (name_len < sizeof(session_name)) {
          strncpy(session_name, entry->d_name + 14, name_len);

          int status = check_daemon_status(session_name);
          if (status == 1) {
            printf("  > %-15s (Attach: ./minishell --client %s)\n",
                   session_name, session_name);
            found = true;
          } else if (status == 0) {
            char full_path[1024];
            snprintf(full_path, sizeof(full_path), "/tmp/%s", entry->d_name);
            unlink(full_path);
          }
        }
      }
    }
  }
  closedir(dir);

  if (!found) {
    printf("  No active sessions found.\n");
  }
  printf("--------------------------------------------------\n");

  return EXIT_SUCCESS;
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

int clear_sessions(void) {
  DIR *dir = opendir("/tmp");
  if (!dir) {
    perror("minishell: cannot open /tmp");
    return EXIT_FAILURE;
  }

  struct dirent *entry;
  int count = 0;

  printf("Clearing minishell sessions...\n");

  while ((entry = readdir(dir)) != NULL) {
    if (strncmp(entry->d_name, "minishell_mux_", 14) == 0) {
      char *ext = strstr(entry->d_name, MUX_SOCKET_EXT);
      if (ext) {
        char session_name[64] = {0};
        size_t name_len = ext - (entry->d_name + 14);

        if (name_len < sizeof(session_name)) {
          strncpy(session_name, entry->d_name + 14, name_len);

          int status = check_daemon_status(session_name);

          if (status == 1) {
            const int sock = socket(AF_UNIX, SOCK_STREAM, 0);
            if (sock != -1) {
              struct sockaddr_un addr;
              memset(&addr, 0, sizeof(addr));
              addr.sun_family = AF_UNIX;
              FORMAT_SOCKET_PATH(addr.sun_path, sizeof(addr.sun_path),
                                 session_name);

              if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == 0) {
                t_msg_header hdr = {.type = MSG_SHUTDOWN, .len = htonl(0)};
                write(sock, &hdr, sizeof(hdr));
              }
              close(sock);
            }
            printf("  [ OK ] Killed session: %s\n", session_name);
            count++;
          } else if (status == 0) {
            printf("  [ OK ] Removed dead socket: %s\n", session_name);
          }

          if (status >= 0) {
            char full_path[1024];
            snprintf(full_path, sizeof(full_path), "/tmp/%s", entry->d_name);
            unlink(full_path);
          }
        }
      }
    }
  }
  closedir(dir);

  if (count == 0) {
    printf("No active sessions found to clear.\n");
  } else {
    printf("Successfully cleared %d sessions.\n", count);
  }

  return EXIT_SUCCESS;
}