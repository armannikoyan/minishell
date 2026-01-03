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

// t_ast_node *upd_tree(t_ast_node *new_node, t_ast_node *head_node) {
//     t_node_type new_node_type;
//     t_node_type head_node_type;
//
//     new_node_type = new_node->type;
//     head_node_type = head_node->type;
//
//     if (head_node_type == NODE_COMMAND && new_node_type == NODE_COMMAND) {
//         print_error("Abstract tree syntax error occurred\n");
//         return NULL;
//     }
//
// }

// Takes a head node of abstract tree and a new node, and returns an updated ast tree.
// If a syntaxis error occurred while updating tree, returns NULL
t_ast_node *ast_build(t_ast_node *new_node, t_ast_node *head_node) {
    if (new_node->type == NODE_COMMAND) {
        printf("New node - NODE_COMMAND: ");
        for (size_t i = 0; new_node->u_data.cmd.argv[i]; ++i)
            printf("%s ", new_node->u_data.cmd.argv[i]);
        printf("\n");
    } else if (new_node->type == NODE_AND || new_node->type == NODE_OR || new_node->type == NODE_PIPE) {
        printf("New node - NODE_BINARY[%s]\n", node_type_to_str(new_node->type));
    } else {
        printf("New node - NODE_REDIR[%s]\n", node_type_to_str(new_node->type));
        printf("Redirection path: %s\n", new_node->u_data.redir.filename);
    }


    return (head_node);
}
