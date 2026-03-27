#include <stdio.h>
#include <stdlib.h>
#include <sys/errno.h>
#include <sys/wait.h>
#include <unistd.h>

#include "error_codes.h"
#include "tokenization.h"
#include "utils.h"

static size_t get_clean_len(const char *str) {
  char quote_state;

  size_t len = 0;
  quote_state = 0;
  for (size_t i = 0; str[i]; ++i) {
    const char prev_state = quote_state;
    set_quote_char(str[i], &quote_state);

    if (prev_state == quote_state)
      ++len;
  }

  return len;
}

char *remove_quotes(char *str) {
  char quote_state;

  if (!str)
    return NULL;

  const size_t new_len = get_clean_len(str);
  char *new_str = malloc(sizeof(char) * (new_len + 1));
  if (!new_str)
    return NULL;

  quote_state = 0;
  for (size_t pos = 0, new_pos = 0; str[pos]; ++pos) {
    const char prev_state = quote_state;
    set_quote_char(str[pos], &quote_state);

    if (prev_state == quote_state) {
      new_str[new_pos] = str[pos];
      ++new_pos;
    }
  }
  new_str[new_len] = '\0';

  return (new_str);
}

int handle_child_exit(const pid_t pid) {
  int status;

  if (waitpid(pid, &status, 0) == -1) {
    if (errno != ECHILD)
      print_error("minishell: waitpid", false);

    return 1;
  }

  if (WIFEXITED(status))
    return (WEXITSTATUS(status));

  if (WIFSIGNALED(status)) {
    const int signum = WTERMSIG(status);

    if (signum == SIGINT || signum == SIGPIPE)
      printf("^C\n");
    else if (signum == SIGQUIT)
      printf("^\\Quit: signum: %d\n", signum);
    return (RESERVED_ERROR_CODES + signum);
  }

  return 1;
}