#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "minishell.h"
#include "mux.h"

int main(const int argc, char **argv, char **envp) {
  if (argc > 1) {
    if (strcmp(argv[1], "--daemon") == 0) {
      const char *sess = (argc > 2) ? argv[2] : "default";
      return run_daemon(envp, sess);
    }
    if (strcmp(argv[1], "--client") == 0) {
      const char *sess = (argc > 2) ? argv[2] : "default";
      return run_client(sess);
    }
    if (strcmp(argv[1], "--list") == 0 || strcmp(argv[1], "-l") == 0) {
      return list_sessions();
    }
    if (strcmp(argv[1], "--clear") == 0 || strcmp(argv[1], "-c") == 0) {
      return clear_sessions();
    }
  }

  if (isatty(STDIN_FILENO) && argc == 1) {
    char unique_session[64];
    snprintf(unique_session, sizeof(unique_session), "tty_%d", getpid());

    printf("[minishell] Starting isolated session: %s\n", unique_session);
    printf("[minishell] To re-attach later, use: ./minishell --client %s\n",
           unique_session);

    return auto_attach_or_spawn(envp, unique_session);
  }

  run_interactive_shell(envp);

  return 0;
}