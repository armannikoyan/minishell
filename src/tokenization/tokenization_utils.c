#include <stdbool.h>
#include <stddef.h>

#include "ast.h"

bool is_redir(const char *str) {
  if (!str)
    return false;

  return (str[0] == '<' || str[0] == '>');
}

bool is_operator(const char *str) {
  if (!str)
    return false;

  return (str[0] == '|' || str[0] == '(' || str[0] == ')' ||
          (str[0] == '&' && str[1] == '&'));
}

size_t get_operator_len(const t_node_type type) {
  switch (type) {
  case HEREDOC_NODE:
  case REDIRECT_APPEND_NODE:
  case OR_NODE:
  case AND_NODE:
    return 2;

  case REDIRECT_IN_NODE:
  case REDIRECT_OUT_NODE:
  case PIPE_NODE:
    return 1;

  default:
    return 0;
  }
}

t_node_type get_node_type(const char *input) {
  if (!input)
    return COMMAND_NODE;

  if (input[0] == '<')
    return (input[1] == '<') ? HEREDOC_NODE : REDIRECT_IN_NODE;

  if (input[0] == '>')
    return (input[1] == '>') ? REDIRECT_APPEND_NODE : REDIRECT_OUT_NODE;

  if (input[0] == '|')
    return (input[1] == '|') ? OR_NODE : PIPE_NODE;

  if (input[0] == '&' && input[1] == '&')
    return AND_NODE;

  if (input[0] == '(')
    return SUBSHELL_NODE;
  if (input[0] == ')')
    return ERROR_NODE;

  return COMMAND_NODE;
}

void set_quote_char(const char c, char *quote_char) {
  if (!quote_char)
    return;
  if (c != '\"' && c != '\'')
    return;

  if (*quote_char == c)
    *quote_char = 0;
  else if (*quote_char == 0)
    *quote_char = c;
}
