#include <stddef.h>

#include "ast.h"
#include "error_codes.h"
#include "utils.h"

int syntax_check(t_ast_node *node, int *errnum) {
  if (!node)
    return SYNTAX_OK;

  if (node->type == SUBSHELL_NODE) {
    if (node->u_data.subshell.root == NULL) {
      print_syntax_error(node, errnum);
      return SYNTAX_ERROR;
    }
    return syntax_check(node->u_data.subshell.root, errnum);
  }

  switch (node->abstract_type) {
  case BIN_NODE:
    if (node->u_data.binary.left == NULL || node->u_data.binary.right == NULL) {
      print_syntax_error(node, errnum);
      return (SYNTAX_ERROR);
    }
    if (syntax_check(node->u_data.binary.left, errnum) != SYNTAX_OK)
      return SYNTAX_ERROR;
    return syntax_check(node->u_data.binary.right, errnum);

  case REDIR_NODE:
    if (node->u_data.redir.filename == NULL ||
        node->u_data.redir.filename[0] == '\0') {
      print_error("minishell: syntax error near token `", true);
      print_error(get_type(node->type), true);
      if (node->type == HEREDOC_NODE)
        print_error("\': No EOL presented\n", true);
      else
        print_error("\': No filename presented\n", true);

      *errnum = SYNTAX_ERROR;
      return SYNTAX_ERROR;
    }
    if (node->u_data.redir.child)
      return syntax_check(node->u_data.redir.child, errnum);
    break;

  default:
    break;
  }

  return SYNTAX_OK;
}
