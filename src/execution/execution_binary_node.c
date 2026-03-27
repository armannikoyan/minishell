#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "ast.h"
#include "error_codes.h"
#include "execution.h"
#include "hash_table.h"
#include "term_settings.h"
#include "utils.h"

extern void free_resources(int code);
extern void close_saved_fds(void);

/*
** CLEANUP HANDLER
** If fork fails, we must close open FDs to trigger SIGPIPE
*/
static int abort_pipeline(const int prev_fd, const int *pipefd) {
  if (prev_fd != -1)
    close(prev_fd);

  if (pipefd) {
    close(pipefd[0]);
    close(pipefd[1]);
  }
  return 1;
}

static void setup_pipe_fds(const int prev_fd, int pipefd[2]) {
  if (prev_fd != -1) {
    dup2(prev_fd, STDIN_FILENO);
    close(prev_fd);
  }

  if (pipefd) {
    dup2(pipefd[1], STDOUT_FILENO);
    close(pipefd[1]);
    close(pipefd[0]);
  }
}

static int wait_for_pipeline(const pid_t last_pid) {
  int status;
  int exit_code = 0;

  signal(SIGINT, SIG_IGN);
  signal(SIGQUIT, SIG_IGN);

  while (1) {
    const pid_t pid = waitpid(-1, &status, 0);
    if (pid == -1) {
      if (errno == ECHILD)
        break;

      print_error("minishell: waitpid", false);
      break;
    }

    if (pid == last_pid) {
      if (WIFEXITED(status)) {
        exit_code = WEXITSTATUS(status);
      } else if (WIFSIGNALED(status)) {
        const int signum = WTERMSIG(status);
        if (signum == SIGINT) {
          fprintf(stderr, "^C\n");
        } else if (signum == SIGQUIT) {
          fprintf(stderr, "^\\Quit: signum: %d\n", signum);
        }
        exit_code = RESERVED_ERROR_CODES + signum;
      }
    }
  }

  psig_set();
  return exit_code;
}

int execute_pipeline(t_ast_node *node, t_hash_table *ht, const int errnum) {
  int pipefd[2] = {-1, -1};
  int prev_fd = -1;
  t_ast_node *curr = node;
  pid_t pid;

  while (curr->type == PIPE_NODE) {
    if (pipe(pipefd) == -1) {
      print_error("minishell: pipe", false);
      return abort_pipeline(prev_fd, NULL);
    }

    pid = fork();
    if (pid == -1) {
      print_error("minishell: fork", false);
      return abort_pipeline(prev_fd, pipefd);
    }

    if (pid == 0) {
      close_saved_fds();
      setup_pipe_fds(prev_fd, pipefd);
      const int code = execute(curr->u_data.binary.left, ht, errnum);
      free_resources(code);
    }

    if (prev_fd != -1)
      close(prev_fd);
    close(pipefd[1]);
    prev_fd = pipefd[0];

    curr = curr->u_data.binary.right;
  }

  pid = fork();
  if (pid == -1) {
    print_error("minishell: fork", false);
    return abort_pipeline(prev_fd, NULL);
  }

  if (pid == 0) {
    close_saved_fds();
    setup_pipe_fds(prev_fd, NULL);
    const int code = execute(curr, ht, errnum);
    free_resources(code);
  }

  if (prev_fd != -1)
    close(prev_fd);

  return wait_for_pipeline(pid);
}