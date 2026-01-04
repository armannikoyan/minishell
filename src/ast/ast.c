#include <stdio.h>

#include "../../includes/ast.h"
#include "utils.h"

const char *node_type_to_str(t_ast_node *node) {
    t_node_type type;

    if (node == NULL)
        return "NODE_UNKNOWN";
    type = node->type;

    if (type == NODE_COMMAND)
        return "COMMAND_NODE";
    if (type == NODE_PIPE)
        return "NODE_PIPE";
    if (type == NODE_AND)
        return "NODE_AND";
    if (type == NODE_OR)
        return "NODE_OR";
    if (type == NODE_REDIRECT_IN)
        return "NODE_REDIRECT_IN";
    if (type == NODE_REDIRECT_OUT)
        return "NODE_REDIRECT_OUT";
    if (type == NODE_REDIRECT_APPEND)
        return "NODE_REDIRECT_APPEND";
    if (type == NODE_HEREDOC)
        return "NODE_HEREDOC";
    return "NODE_UNKNOWN";
}

t_ast_node *head_is_comand_node(t_ast_node *node, t_ast_node *head) {
    if (node->abstract_type == BINARY_NODE) {
        node->u_data.binary.left = head;
        return node;
    }
    if (node->abstract_type == REDIRECTION_NODE) {
        node->u_data.redir.child = head;
        return node;
    }
    //TODO: make a normal error
    print_error("Abstract tree syntax error occurred: impossible node combination\n");
    return NULL;
}

t_ast_node *head_is_binary_node(t_ast_node *node, t_ast_node *head) {
    t_ast_node *iter;

    if (node->abstract_type == COMMAND_NODE) {
        if (head->u_data.binary.right == NULL && head->u_data.binary.left != NULL) {
            head->u_data.binary.right = node;
            return head;
        }
        iter = head->u_data.binary.right;
        if (iter != NULL && iter->abstract_type == REDIRECTION_NODE) {
            while (iter->u_data.redir.child != NULL && iter->u_data.redir.child->abstract_type == REDIRECTION_NODE)
                iter = iter->u_data.redir.child;
            if (iter->u_data.redir.child == NULL) {
                iter->u_data.redir.child = node;
                return head;
            }
        }
    }
    if (node->abstract_type == BINARY_NODE) {
        if (head->u_data.binary.right != NULL && head->u_data.binary.left != NULL) {
            node->u_data.binary.left = head;
            return node;
        }
    }
    if (node->abstract_type == REDIRECTION_NODE) {
        if (head->u_data.binary.right == NULL) {
            head->u_data.binary.right = node;
            return head;
        }
        if (head->u_data.binary.right->abstract_type == COMMAND_NODE ||
            head->u_data.binary.right->abstract_type == REDIRECTION_NODE) {
            node->u_data.redir.child = head->u_data.binary.right;
            head->u_data.binary.right = node;
            return head;
        }
    }
    //TODO: make a normal error
    print_error("Abstract tree syntax error occurred: impossible node combination\n");
    return NULL;
}

t_ast_node *head_is_redir_node(t_ast_node *node, t_ast_node *head) {
    t_ast_node *iter;

    if (node->abstract_type == REDIRECTION_NODE) {
        node->u_data.redir.child = head;
        return node;
    }
    if (node->abstract_type == BINARY_NODE) {
        node->u_data.binary.left = head;
        return node;
    }
    iter = head;
    if (node->abstract_type == COMMAND_NODE) {
        while (iter->u_data.redir.child != NULL && iter->u_data.redir.child->abstract_type == REDIRECTION_NODE)
            iter = iter->u_data.redir.child;
        if (iter->u_data.redir.child == NULL) {
            iter->u_data.redir.child = node;
            return head;
        }
    }
    //TODO: make a normal error
    print_error("Abstract tree syntax error occurred: impossible node combination\n");
    return NULL;
}


t_ast_node *upd_tree(t_ast_node *new_node, t_ast_node *head_node) {
    if (head_node == NULL)
        return new_node;

    if (head_node->abstract_type == COMMAND_NODE)
        return head_is_comand_node(new_node, head_node);
    if (head_node->abstract_type == BINARY_NODE)
        return head_is_binary_node(new_node, head_node);
    if (head_node->abstract_type == REDIRECTION_NODE)
        return head_is_redir_node(new_node, head_node);
    //TODO: make a normal error
    print_error("Abstract tree syntax error occurred: impossible node combination\n");
    return NULL;
}

// Takes a head node of abstract tree and a new node, and returns an updated ast tree.
// If a syntaxis error occurred while updating tree, returns NULL
t_ast_node *ast_build(t_ast_node *new_node, t_ast_node *head_node) {
    if (new_node->abstract_type == COMMAND_NODE) {
        printf("New node - COMMAND_NODE: ");
        for (size_t i = 0; new_node->u_data.cmd.argv[i]; ++i)
            printf("%s ", new_node->u_data.cmd.argv[i]);
        printf("\n");
    } else if (new_node->abstract_type == BINARY_NODE) {
        printf("New node - BINARY_NODE[%s]\n", node_type_to_str(new_node));
    } else if (new_node->abstract_type == REDIRECTION_NODE) {
        printf("New node - REDIRECTION_NODE[%s]\n", node_type_to_str(new_node));
        printf("Redirection path: %s\n", new_node->u_data.redir.filename);
    } else {
        printf("New node - UNKNOWN_NODE\n");
    }


    return upd_tree(new_node, head_node);
}
