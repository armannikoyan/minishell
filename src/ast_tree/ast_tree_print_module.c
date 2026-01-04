#include <stdio.h>
#include "ast_tree.h"

// Return a name of a node for printing abstract tree
static const char *get_node_name(t_node_type type) {
    if (type == NODE_COMMAND)
        return "CMD";
    if (type == NODE_PIPE)
        return "PIPE";
    if (type == NODE_AND)
        return "AND";
    if (type == NODE_OR)
        return "OR";
    if (type == NODE_REDIRECT_IN)
        return "REDIR_IN";
    if (type == NODE_REDIRECT_OUT)
        return "REDIR_OUT";
    if (type == NODE_REDIRECT_APPEND)
        return "REDIR_APPEND";
    if (type == NODE_HEREDOC)
        return "HEREDOC";
    return "UNKNOWN";
}

// Prints argv for command node
static void print_argv(char **argv) {
    int i;

    i = 0;
    printf(": ");
    if (!argv)
        return;
    while (argv[i]) {
        printf("%s", argv[i]);
        if (argv[i + 1])
            printf(" ");
        i++;
    }
}

// Prints accumulated prefix
static void print_prefix(int depth, int *has_pipe) {
    int i;

    i = 0;
    while (i < depth) {
        if (has_pipe[i])
            printf("|   ");
        else
            printf("    ");
        i++;
    }
}

// Recursive printing of abstract tree with ASCII-marking
static void print_ast_rec(t_ast_node *node, int depth, int *has_pipe, int is_last) {
    if (!node)
        return;
    print_prefix(depth, has_pipe);
    if (is_last)
        printf("`-- ");
    else
        printf("|-- ");
    printf("%s", get_node_name(node->type));
    if (node->type == NODE_COMMAND)
        print_argv(node->u_data.cmd.argv);
    else if (get_node_type_abstraction(node) == REDIRECTION_NODE)
        printf(": %s", node->u_data.redir.filename);
    printf("\n");
    has_pipe[depth] = !is_last;
    if (get_node_type_abstraction(node) == BINARY_NODE) {
        print_ast_rec(node->u_data.binary.left, depth + 1, has_pipe, 0);
        print_ast_rec(node->u_data.binary.right, depth + 1, has_pipe, 1);
    } else if (get_node_type_abstraction(node) == REDIRECTION_NODE) {
        print_ast_rec(node->u_data.redir.child, depth + 1, has_pipe, 1);
    }
}

// Prints abstract syntax tree
void print_ast(t_ast_node *root) {
    int has_pipe[64];
    int i;

    if (!root)
        return;
    i = 0;
    while (i < 64)
        has_pipe[i++] = 0;
    print_ast_rec(root, 0, has_pipe, 1);
}
