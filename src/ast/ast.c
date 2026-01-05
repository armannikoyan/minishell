#include <stdio.h>

#include "../../includes/ast.h"
#include "utils.h"

const char *node_type_to_str(t_ast_node *node) {
    t_node_type type;

    if (node == NULL)
        return "UNKNOWN_NODE";
    type = node->type;

    if (type == COMMAND_NODE)
        return "COMMAND_NODE";
    if (type == PIPE_NODE)
        return "PIPE_NODE";
    if (type == AND_NODE)
        return "AND_NODE";
    if (type == OR_NODE)
        return "OR_NODE";
    if (type == REDIRECT_IN_NODE)
        return "REDIRECT_IN_NODE";
    if (type == REDIRECT_OUT_NODE)
        return "REDIRECT_OUT_NODE";
    if (type == REDIRECT_APPEND_NODE)
        return "REDIRECT_APPEND_NODE";
    if (type == HEREDOC_NODE)
        return "HEREDOC_NODE";
    return "UNKNOWN_NODE";
}

t_ast_node *head_is_comand_node(t_ast_node *node, t_ast_node *root) {
    if (node->abstract_type == BIN_NODE) {
        node->u_data.binary.left = root;
        return node;
    }
    if (node->abstract_type == REDIR_NODE) {
        node->u_data.redir.child = root;
        return node;
    }
    //TODO: make a normal error
    print_error("Abstract tree syntax error occurred: impossible node combination\n");
    return NULL;
}

t_ast_node *head_is_binary_node(t_ast_node *node, t_ast_node *root) {
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
    print_error("Abstract tree syntax error occurred: impossible node combination\n");
    return NULL;
}

t_ast_node *head_is_redir_node(t_ast_node *node, t_ast_node *root) {
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
    print_error("Abstract tree syntax error occurred: impossible node combination\n");
    return NULL;
}


t_ast_node *upd_tree(t_ast_node *new_node, t_ast_node *root) {
    if (root == NULL)
        return new_node;

    if (root->abstract_type == CMD_NODE)
        return head_is_comand_node(new_node, root);
    if (root->abstract_type == BIN_NODE)
        return head_is_binary_node(new_node, root);
    if (root->abstract_type == REDIR_NODE)
        return head_is_redir_node(new_node, root);
    //TODO: make a normal error
    print_error("Abstract tree syntax error occurred: impossible node combination\n");
    return NULL;
}

// Takes a head node of abstract tree and a new node, and returns an updated ast tree.
// If a syntaxis error occurred while updating tree, returns NULL
t_ast_node *ast_build(t_ast_node *new_node, t_ast_node *root) {
    if (new_node->abstract_type == CMD_NODE) {
        printf("New node - COMMAND_NODE: ");
        for (size_t i = 0; new_node->u_data.cmd.argv[i]; ++i)
            printf("%s ", new_node->u_data.cmd.argv[i]);
        printf("\n");
    } else if (new_node->abstract_type == BIN_NODE) {
        printf("New node - BINARY_NODE[%s]\n", node_type_to_str(new_node));
    } else if (new_node->abstract_type == REDIR_NODE) {
        printf("New node - REDIRECTION_NODE[%s]\n", node_type_to_str(new_node));
        printf("Redirection path: %s\n", new_node->u_data.redir.filename);
    } else {
        printf("New node - UNKNOWN_NODE\n");
    }


    return upd_tree(new_node, root);
}
