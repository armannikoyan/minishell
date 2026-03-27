#include <stdio.h>

#include "ast.h"
#include "error_codes.h"
#include "utils.h"

char *get_binary_node_by_type(const int type) {
  switch (type) {
  case AND_NODE:
    return "&&";
  case OR_NODE:
    return "||";
  case PIPE_NODE:
    return "|";
  default:;
  }

  return "NULL";
}

void print_syntax_error(t_ast_node *node, int *errnum) {
  if (!node)
    return;

  *errnum = SYNTAX_ERROR;
  print_error("minishell: syntax error near unexpected token `", true);

  if (node->abstract_type == BIN_NODE || node->abstract_type == REDIR_NODE)
    print_error(get_binary_node_by_type(node->type), true);
  else if (node->type == COMMAND_NODE) {
    if (node->u_data.cmd.argv && node->u_data.cmd.argv[0])
      print_error(node->u_data.cmd.argv[0], true);
  } else if (node->type == SUBSHELL_NODE)
    print_syntax_error(node->u_data.subshell.root, errnum);

  print_error("\'\n", true);
}

static int get_precedence(const t_node_type type) {
  switch (type) {
  case PIPE_NODE:
    return 2;

  case AND_NODE:
  case OR_NODE:
    return 1;

  default:
    return 0;
  }
}

static t_ast_node *handle_cmd_root(t_ast_node *new_node, t_ast_node *root,
                                   int *errnum) {
  if (new_node->abstract_type == BIN_NODE) {
    new_node->u_data.binary.left = root;
    return new_node;
  }
  if (new_node->abstract_type == REDIR_NODE) {
    new_node->u_data.redir.child = root;
    return new_node;
  }

  print_syntax_error(new_node, errnum);
  return NULL;
}

static t_ast_node *handle_bin_root(t_ast_node *new_node, t_ast_node *root,
                                   int *errnum) {
  if (new_node->abstract_type == CMD_NODE ||
      new_node->abstract_type == REDIR_NODE) {
    if (root->u_data.binary.right != NULL) {
      root->u_data.binary.right =
          ast_build(new_node, root->u_data.binary.right, errnum);
    } else
      root->u_data.binary.right = new_node;

    return root;
  }
  if (new_node->abstract_type == BIN_NODE) {
    if (get_precedence(new_node->type) > get_precedence(root->type)) {
      root->u_data.binary.right =
          ast_build(new_node, root->u_data.binary.right, errnum);

      return root;
    }

    new_node->u_data.binary.left = root;
    return new_node;
  }

  print_syntax_error(new_node, errnum);
  return NULL;
}

static t_ast_node *handle_redir_root(t_ast_node *new_node, t_ast_node *root,
                                     int *errnum) {
  if (new_node->abstract_type == BIN_NODE) {
    new_node->u_data.binary.left = root;
    return new_node;
  }

  t_ast_node *last_redir = root;
  while (last_redir->u_data.redir.child != NULL &&
         last_redir->u_data.redir.child->abstract_type == REDIR_NODE)
    last_redir = last_redir->u_data.redir.child;

  if (new_node->abstract_type == REDIR_NODE) {
    new_node->u_data.redir.child = last_redir->u_data.redir.child;
    last_redir->u_data.redir.child = new_node;

    return root;
  }

  if (new_node->abstract_type == CMD_NODE) {
    if (last_redir->u_data.redir.child == NULL) {
      last_redir->u_data.redir.child = new_node;
      return root;
    }
  }

  print_syntax_error(new_node, errnum);
  return NULL;
}

// Takes a head node of abstract tree and a new node, and returns an updated ast
// tree. If a syntaxis error occurred while updating tree, returns NULL
t_ast_node *ast_build(t_ast_node *new_node, t_ast_node *root, int *errnum) {
  if (root == NULL)
    return new_node;

  switch (root->abstract_type) {
  case CMD_NODE:
    return handle_cmd_root(new_node, root, errnum);

  case BIN_NODE:
    return handle_bin_root(new_node, root, errnum);

  case REDIR_NODE:
    return handle_redir_root(new_node, root, errnum);

  default:
    print_syntax_error(new_node, errnum);
    return NULL;
  }
}
