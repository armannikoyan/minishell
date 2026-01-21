#include <stdio.h>

#include "ast.h"

const char *get_type(t_node_type type) {
    if (type == PIPE_NODE)
        return "|";
    if (type == AND_NODE)
        return "&&";
    if (type == OR_NODE)
        return "||";
    if (type == REDIRECT_IN_NODE)
        return "<";
    if (type == REDIRECT_OUT_NODE)
        return ">";
    if (type == REDIRECT_APPEND_NODE)
        return ">>";
    if (type == HEREDOC_NODE)
        return "<<";
    return "unknown";
}

// Prints current node info and abstract syntax tree structure
void print_ast_info(t_ast_node *root, t_ast_node *curr_node) {
    char *str;

    str = "New node - ";
    if (curr_node->abstract_type == CMD_NODE) {
        if (curr_node->type == COMMAND_NODE) {
            printf("%sCOMMAND_NODE: ", str);
            for (size_t i = 0; curr_node->u_data.cmd.argv[i]; ++i)
                printf("%s ", curr_node->u_data.cmd.argv[i]);
            printf("\n");
        } else
            printf("%sSUBSHELL_NODE: it contains a previous tree\n", str);
    }
    else if (curr_node->abstract_type == BIN_NODE)
        printf("%sBINARY_NODE[%s]\n", str, get_type(curr_node->type));
    else if (curr_node->abstract_type == REDIR_NODE) {
        printf("%sREDIRECTION_NODE[%s]\n", str, get_type(curr_node->type));
        printf("Redirection path: %s\n", curr_node->u_data.redir.filename);
    }
    else
        printf("%sUNKNOWN_NODE\n", str);

    print_ast(root);
    printf("\n\n");
}
