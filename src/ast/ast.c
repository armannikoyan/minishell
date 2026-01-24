#include <stdio.h>

#include "utils.h"
#include "ast.h"

#include "error_codes.h"

void print_syntax_error(t_ast_node *node, int *errnum) {
    if (!node)
        return ;
    *errnum = SYNTAX_ERROR;
    print_error("minishell: syntax error near unexpected token `", true);
    if (node->abstract_type == BIN_NODE || node->abstract_type == REDIR_NODE)
        print_error(get_type(node->type), true);
    else if (node->type == COMMAND_NODE) {
        if (node->u_data.cmd.argv)
            print_error(node->u_data.cmd.argv[0], true);
    }
    else if (node->type == SUBSHELL_NODE)
        print_syntax_error(node->u_data.subshell.root, errnum);
    print_error("'\n", true);
}

static int get_precedence(t_node_type type) {
    if (type == PIPE_NODE)
        return 2;
    if (type == AND_NODE || type == OR_NODE)
        return 1;
    return 0;
}

static t_ast_node *root_is_command_node(t_ast_node *node, t_ast_node *root, int *errnum) {
    if (node->abstract_type == BIN_NODE) {
        node->u_data.binary.left = root;
        return node;
    }
    if (node->abstract_type == REDIR_NODE) {
        node->u_data.redir.child = root;
        return node;
    }
    //TODO: make a normal error
    *errnum = SYNTAX_ERROR;
    print_syntax_error(node, errnum);
    return NULL;
}

static t_ast_node *root_is_binary_node(t_ast_node *node, t_ast_node *root, int *errnum) {
    if (node->abstract_type == CMD_NODE || node->abstract_type == REDIR_NODE) {
        if (root->u_data.binary.right != NULL) {
            root->u_data.binary.right = ast_build(node, root->u_data.binary.right, errnum);
            return root;
        }
        root->u_data.binary.right = node;
        return root;
    }
    if (node->abstract_type == BIN_NODE) {
        int root_prec = get_precedence(root->type);
        int node_prec = get_precedence(node->type);
        if (node_prec > root_prec) {
            root->u_data.binary.right = ast_build(node, root->u_data.binary.right, errnum);
            return root;
        }
        node->u_data.binary.left = root;
        return node;
    }
    //TODO: make a normal error
    *errnum = SYNTAX_ERROR;
    print_syntax_error(node, errnum);
    return NULL;
}

static t_ast_node *root_is_redir_node(t_ast_node *node, t_ast_node *root, int *errnum) {
    t_ast_node *iter;

    if (node->abstract_type == REDIR_NODE) {
        node->u_data.redir.child = root;
        return node;
    }
    if (node->abstract_type == BIN_NODE) {
        node->u_data.binary.left = root;
        return node;
    }
    iter = root;
    if (node->abstract_type == CMD_NODE) {
        while (iter->u_data.redir.child != NULL && iter->u_data.redir.child->abstract_type == REDIR_NODE)
            iter = iter->u_data.redir.child;
        if (iter->u_data.redir.child == NULL) {
            iter->u_data.redir.child = node;
            return root;
        }
    }
    //TODO: make a normal error
    *errnum = SYNTAX_ERROR;
    print_syntax_error(node, errnum);
    return NULL;
}

// Takes a head node of abstract tree and a new node, and returns an updated ast tree.
// If a syntaxis error occurred while updating tree, returns NULL
t_ast_node *ast_build(t_ast_node *new_node, t_ast_node *root, int *errnum) {
    if (root == NULL)
        return new_node;

    if (root->abstract_type == CMD_NODE)
        return root_is_command_node(new_node, root, errnum);
    if (root->abstract_type == BIN_NODE)
        return root_is_binary_node(new_node, root, errnum);
    if (root->abstract_type == REDIR_NODE)
        return root_is_redir_node(new_node, root, errnum);
    //TODO: make a normal error
    *errnum = SYNTAX_ERROR;
    print_syntax_error(new_node, errnum);
    return NULL;
}
