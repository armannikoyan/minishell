#include <stdlib.h>

#include "ast.h"
#include "utils.h"

static void command_node_deletion(t_ast_node *node) {
  switch (node->type) {
  case COMMAND_NODE:
    free_split(node->u_data.cmd.argv);
    break;
  case SUBSHELL_NODE:
    ast_deletion(node->u_data.subshell.root);
    break;
  default:;
  }

  free(node);
}

static void binary_node_deletion(t_ast_node *node) {
  ast_deletion(node->u_data.binary.left);
  ast_deletion(node->u_data.binary.right);
  free(node);
}

static void redir_node_deletion(t_ast_node *node) {
  ast_deletion(node->u_data.redir.child);
  free(node->u_data.redir.filename);
  free(node);
}

void ast_deletion(t_ast_node *root) {
  if (!root)
    return;

  switch (root->abstract_type) {
  case CMD_NODE:
    command_node_deletion(root);
    break;
  case BIN_NODE:
    binary_node_deletion(root);
    break;
  case REDIR_NODE:
    redir_node_deletion(root);
    break;
  default:;
  }

  root = NULL;
}
