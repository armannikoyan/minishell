#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "ast.h"
#include "execution.h"
#include "hash_table.h"
#include "term_settings.h"
#include "utils.h"

static int execute_pipe_helper(int *filedes, pid_t *pid, t_ast_node *node, t_hash_table *ht, int errnum) {
    int err_code;

    if (pid[0] == 0) {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        if (close(filedes[0]) == -1) {
            print_error("minishell: close", false);
            return (1);
        }
        if (dup2(filedes[1], STDOUT_FILENO) == -1) {
            print_error("minishell: dup2", false);
            return (1);
        }
        if (close(filedes[1]) == -1) {
            print_error("minishell: close", false);
            return (1);
        }
        exit(execute(node->u_data.binary.left, ht, errnum));
    }
    if (pid[0] && pid[1]) {
        signal(SIGINT, SIG_IGN);
        signal(SIGQUIT, SIG_IGN);
        if (close(filedes[0]) == -1) {
            print_error("minishell: close", false);
            return (1);
        }
        if (close(filedes[1]) == -1) {
            print_error("minishell: close", false);
            return (1);
        }
        handle_child_exit(pid[0]);
        err_code = handle_child_exit(pid[1]);
        psig_set();
        return (err_code);
    }
    else {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        if (close(filedes[1]) == -1) {
            print_error("minishell: close", false);
            return (1);
        }
        if (dup2(filedes[0], STDIN_FILENO) == -1) {
            print_error("minishell: dup2", false);
            return (1);
        }
        if (close(filedes[0]) == -1) {
            print_error("minishell: close", false);
            return (1);
        }
        exit(execute(node->u_data.binary.right, ht, errnum));
    }
}

static int    execute_pipe(t_ast_node *node, t_hash_table *ht, int errnum) {
    int filedes[2];
    pid_t pid[2];

    if (pipe(filedes) == -1) {
        // TODO: Write Normal Error
        print_error("minishell: pipe", false);
        return (1);
    }
    pid[0] = fork();
    if (pid[0] == -1) {
        // TODO: write normal error
        print_error("minishell: fork", false);
        return (1);
    }
    if (pid[0]) {
        pid[1] = fork();
        if (pid[1] == -1) {
            // TODO: write normal error
            print_error("minishell: fork", false);
            return (1);
        }
    }
    return (execute_pipe_helper(filedes, pid, node, ht, errnum));
}

int    execute_binary(t_ast_node *node, t_hash_table *ht, int errnum) {
    int err_code;

    err_code = 0;
    if (node->type == OR_NODE) {
        err_code = execute(node->u_data.binary.left, ht, errnum);
        if (err_code != 0) {
            return (execute(node->u_data.binary.right, ht, errnum));
        }
    }
    else if (node->type == AND_NODE) {
        err_code = execute(node->u_data.binary.left, ht, errnum);
        if (err_code == 0) {
            return (execute(node->u_data.binary.right, ht, errnum));
        }
    }
    else if (node->type == PIPE_NODE)
        return (execute_pipe(node, ht, errnum));
    return (err_code);
}