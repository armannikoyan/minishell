#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "ast.h"
#include "hash_table.h"
#include "utils.h"
#include "execution.h"

#include "term_settings.h"

void    execute_subshell(t_ast_node *node, t_hash_table *ht) {
    pid_t pid;

    pid = fork();
    if (pid == -1) {
        // TODO: write normal error
        print_error("minishell: fork", false);
        return ;
    }
    if (pid) {
        signal(SIGINT, SIG_IGN);
        signal(SIGQUIT, SIG_IGN);
        handle_child_exit(pid);
        psig_set();
    }
    else {
        execute(node->u_data.subshell.root, ht);
        exit(errno);
    }
}

void    execute(t_ast_node *node, t_hash_table *ht) {
    if (node->abstract_type == CMD_NODE && node->type == COMMAND_NODE)
        execute_command(node, ht);
    else if (node->abstract_type == BIN_NODE)
        execute_binary(node, ht);
    else if (node->abstract_type == REDIR_NODE)
        execute_redir(node, ht);
    else if (node->type == SUBSHELL_NODE)
        execute_subshell(node, ht);
}
