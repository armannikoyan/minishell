#include <stddef.h>
#include <stdlib.h>

#include "../../includes/ast.h"


#include <stdio.h> // remove

#include "utils.h"

const char *node_type_to_str(t_node_type type) {
    switch (type) {
        case NODE_COMMAND: return "NODE_COMMAND";
        case NODE_PIPE: return "NODE_PIPE";
        case NODE_AND: return "NODE_AND";
        case NODE_OR: return "NODE_OR";
        case NODE_REDIRECT_IN: return "NODE_REDIRECT_IN";
        case NODE_REDIRECT_OUT: return "NODE_REDIRECT_OUT";
        case NODE_REDIRECT_APPEND: return "NODE_REDIRECT_APPEND";
        case NODE_HEREDOC: return "NODE_HEREDOC";
        default: return "NODE_UNKNOWN";
    }
}

t_node_type_abstraction get_node_type_abstraction(t_node_type type) {
    if (type == NODE_COMMAND)
        return COMMAND_NODE;
    if (type == NODE_PIPE)
        return BINARY_NODE;
    if (type == NODE_AND)
        return BINARY_NODE;
    if (type == NODE_OR)
        return BINARY_NODE;
    if (type == NODE_REDIRECT_IN)
        return REDIRECTION_NODE;
    if (type == NODE_REDIRECT_OUT)
        return REDIRECTION_NODE;
    if (type == NODE_REDIRECT_APPEND)
        return REDIRECTION_NODE;
    if (type == NODE_HEREDOC)
        return REDIRECTION_NODE;
    return UNDEFINED_NODE;
}

t_ast_node *ast_tree_error() {
    print_error("Abstract tree syntax error occurred: impossible node combination\n");
    return NULL;
}

int is_binary_node_full(t_ast_node *node) {
    int right_full;
    int left_full;

    right_full = node->u_data.binary.right != NULL;
    left_full = node->u_data.binary.left != NULL;
    return right_full && left_full;
}

int is_binary_node_filled_right(t_ast_node *node) {
    int right_correct;
    int left_correct;

    right_correct = node->u_data.binary.right == NULL;
    left_correct = node->u_data.binary.left != NULL;
    return right_correct && left_correct;
}


t_ast_node *upd_tree(t_ast_node *new_node, t_ast_node *head_node) {
    t_node_type_abstraction new_node_type;
    t_node_type_abstraction head_node_type;

    if (head_node == NULL)
        return new_node;
    new_node_type = get_node_type_abstraction(new_node->type);
    head_node_type = get_node_type_abstraction(head_node->type);

    if (head_node_type == COMMAND_NODE) {
        if (new_node_type == BINARY_NODE) {
            new_node->u_data.binary.left = head_node;
            return new_node;
        }
    }
    if (head_node_type == BINARY_NODE) {
        if (new_node_type == COMMAND_NODE) {
            if (is_binary_node_filled_right(head_node)) {
                head_node->u_data.binary.right = new_node;
                return head_node;
            }
        }
        if (new_node_type == BINARY_NODE) {
            if (is_binary_node_full(head_node)) {
                new_node->u_data.binary.left = head_node;
                return new_node;
            }
        }
    }
    return ast_tree_error();
}

// Takes a head node of abstract tree and a new node, and returns an updated ast tree.
// If a syntaxis error occurred while updating tree, returns NULL
t_ast_node *ast_build(t_ast_node *new_node, t_ast_node *head_node) {
    t_node_type_abstraction node_type_abstraction;

    node_type_abstraction = get_node_type_abstraction(new_node->type);
    if (node_type_abstraction == COMMAND_NODE) {
        printf("New node - COMMAND_NODE: ");
        for (size_t i = 0; new_node->u_data.cmd.argv[i]; ++i)
            printf("%s ", new_node->u_data.cmd.argv[i]);
        printf("\n");
    } else if (node_type_abstraction == BINARY_NODE) {
        printf("New node - BINARY_NODE[%s]\n", node_type_to_str(new_node->type));
    } else if (node_type_abstraction == REDIRECTION_NODE) {
        printf("New node - REDIRECTION_NODE[%s]\n", node_type_to_str(new_node->type));
        printf("Redirection path: %s\n", new_node->u_data.redir.filename);
    } else {
        printf("New node - UNKNOWN_NODE\n");
    }


    return upd_tree(new_node, head_node);
}
