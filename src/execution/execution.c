#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "ast.h"
#include "hash_table.h"
#include "utils.h"
#include "execution.h"

#include "term_settings.h"

int    execute_subshell(t_ast_node *node, t_hash_table *ht, int errnum) {
    pid_t pid;
    int status;

    pid = fork();
    if (pid == -1) {
        // TODO: write normal error
        print_error("minishell: fork", false);
        return (1);
    }
    if (pid) {
        signal(SIGINT, SIG_IGN);
        signal(SIGQUIT, SIG_IGN);
        status = handle_child_exit(pid);
        psig_set();
        return (status);
    }
    else
        exit(execute(node->u_data.subshell.root, ht, errnum));
}

int execute(t_ast_node *node, t_hash_table *ht, int errnum) {
    if (!node)
        return (0);
    if (node->abstract_type == CMD_NODE && node->type == COMMAND_NODE)
        return (execute_command(node, ht, errnum));
    if (node->abstract_type == BIN_NODE)
        return (execute_binary(node, ht, errnum));
    if (node->abstract_type == REDIR_NODE)
        return (execute_redir(node, ht, errnum));
    if (node->type == SUBSHELL_NODE)
        return (execute_subshell(node, ht, errnum));
    return (0);
}
