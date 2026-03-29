#include <string.h>
#include <unistd.h>

#include "minishell.h"
#include "mux.h"

int main(const int argc, char **argv, char **envp) {
  const char *session_name = "default";

  if (argc > 1) {
    if (strcmp(argv[1], "--daemon") == 0) {
      if (argc > 2)
        session_name = argv[2];
      return run_daemon(envp, session_name);
    }
    if (strcmp(argv[1], "--client") == 0) {
      if (argc > 2)
        session_name = argv[2];
      return run_client(session_name);
    }
  }

  if (isatty(STDIN_FILENO) && argc == 1) {
    return auto_attach_or_spawn(envp, session_name);
  }

  run_interactive_shell(envp);

  return 0;
}