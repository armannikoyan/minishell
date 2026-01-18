#include <stdio.h>

#include "utils.h"
#include "ast.h"

static t_ast_node *root_is_command_node(t_ast_node *node, t_ast_node *root) {
    if (node->abstract_type == BIN_NODE) {
        node->u_data.binary.left = root;
        return node;
    }
    if (node->abstract_type == REDIR_NODE) {
        node->u_data.redir.child = root;
        return node;
    }
    //TODO: make a normal error
    print_error("Abstract tree syntax error occurred: impossible node combination\n", true);
    return NULL;
}

static t_ast_node *root_is_binary_node(t_ast_node *node, t_ast_node *root) {
    t_ast_node *iter;

    if (node->abstract_type == CMD_NODE) {
        if (root->u_data.binary.right == NULL && root->u_data.binary.left != NULL) {
            root->u_data.binary.right = node;
            return root;
        }
        iter = root->u_data.binary.right;
        if (iter != NULL && iter->abstract_type == REDIR_NODE) {
            while (iter->u_data.redir.child != NULL && iter->u_data.redir.child->abstract_type == REDIR_NODE)
                iter = iter->u_data.redir.child;
            if (iter->u_data.redir.child == NULL) {
                iter->u_data.redir.child = node;
                return root;
            }
        }
    }
    if (node->abstract_type == BIN_NODE) {
        if (root->u_data.binary.right != NULL && root->u_data.binary.left != NULL) {
            node->u_data.binary.left = root;
            return node;
        }
    }
    if (node->abstract_type == REDIR_NODE) {
        if (root->u_data.binary.right == NULL) {
            root->u_data.binary.right = node;
            return root;
        }
        if (root->u_data.binary.right->abstract_type == CMD_NODE ||
            root->u_data.binary.right->abstract_type == REDIR_NODE) {
            node->u_data.redir.child = root->u_data.binary.right;
            root->u_data.binary.right = node;
            return root;
        }
    }
    //TODO: make a normal error
    print_error("Abstract tree syntax error occurred: impossible node combination\n", true);
    return NULL;
}

static t_ast_node *root_is_redir_node(t_ast_node *node, t_ast_node *root) {
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
    print_error("Abstract tree syntax error occurred: impossible node combination\n", true);
    return NULL;
}

// Takes a head node of abstract tree and a new node, and returns an updated ast tree.
// If a syntaxis error occurred while updating tree, returns NULL
t_ast_node *ast_build(t_ast_node *new_node, t_ast_node *root) {
    if (root == NULL)
        return new_node;

    if (root->abstract_type == CMD_NODE)
        return root_is_command_node(new_node, root);
    if (root->abstract_type == BIN_NODE)
        return root_is_binary_node(new_node, root);
    if (root->abstract_type == REDIR_NODE)
        return root_is_redir_node(new_node, root);
    //TODO: make a normal error
    print_error("Abstract tree syntax error occurred: impossible node combination\n", true);
    return NULL;
}
