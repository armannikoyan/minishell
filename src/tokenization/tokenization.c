#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>

#include "error_codes.h"
#include "tokenization.h"
#include "utils.h"

static t_ast_node *construct_subshell_node(const char *input, size_t *pos,
                                           bool *skip_iteration, int *errnum) {
  char *content = extract_subshell_content(input, pos, errnum);
  if (!content)
    return NULL;

  if (content[0] == '\0') {
    print_error("minishell: parsing error near unexpected token `)\'\n", true);
    *errnum = SYNTAX_ERROR;
    free(content);
    return NULL;
  }

  t_ast_node *child_ast = tokenize(content, errnum);
  free(content);

  if (!child_ast) {
    *skip_iteration = true;
    return NULL;
  }

  return create_subshell_node(SUBSHELL_NODE, child_ast);
}

static t_ast_node *construct_node(char *input, size_t *pos,
                                  const t_node_type type, bool *skip_iteration,
                                  int *errnum) {
  switch (type) {
  case COMMAND_NODE:
    return create_cmd_node(type, get_argv(input, pos));

  case PIPE_NODE:
  case AND_NODE:
  case OR_NODE:
    *pos += get_operator_len(type);
    return create_binary_node(type);

  case REDIRECT_IN_NODE:
  case REDIRECT_OUT_NODE:
  case REDIRECT_APPEND_NODE:
  case HEREDOC_NODE:
    *pos += get_operator_len(type);
    return create_redir_node(type, substr_next(input, pos));

  case SUBSHELL_NODE:
    return construct_subshell_node(input, pos, skip_iteration, errnum);

  case ERROR_NODE:
  default:
    print_error("minishell: parsing error near unexpected token `)\'\n", true);
    *errnum = SYNTAX_ERROR;
    return NULL;
  }
}

t_ast_node *tokenize(char *input, int *errnum) {
  size_t pos = 0;
  bool skip_iteration;
  t_ast_node *node = NULL;
  t_ast_node *head_node = NULL;

  while (input[pos]) {
    while (isspace((unsigned char)input[pos]))
      pos++;

    if (!input[pos])
      break;

    skip_iteration = false;
    node = construct_node(input, &pos, get_node_type(input + pos),
                          &skip_iteration, errnum);

    if (skip_iteration)
      continue;

    if (node)
      head_node = ast_build(node, head_node, errnum);
    else {
      if (head_node)
        ast_deletion(head_node);

      return NULL;
    }
  }

  return head_node;
}
