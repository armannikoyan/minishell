#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/_types/_pid_t.h>

#include "ast.h"
#include "execution.h"
#include "hash_table.h"
#include "utils.h"

static void execute_pipe_helper(int *filedes, pid_t *pid, t_ast_node *node, t_hash_table *ht) {
    if (pid[0] == 0) {
        if (close(filedes[0]) == -1) {
            print_error("minishell: close", false);
            return ;
        }
        if (dup2(filedes[1], STDOUT_FILENO) == -1) {
            print_error("minishell: dup2", false);
            return;
        }
        if (close(filedes[1]) == -1) {
            print_error("minishell: close", false);
            return ;
        }
        execute(node->u_data.binary.left, ht);
        exit(errno);
    }
    if (pid[0] && pid[1]) {
        if (close(filedes[0]) == -1) {
            print_error("minishell: close", false);
            return ;
        }
        if (close(filedes[1]) == -1) {
            print_error("minishell: close", false);
            return ;
        }
        if (waitpid(pid[0], NULL, 0) == -1) {
            // TODO: write normal error
            print_error("minishell: waitpid", false);
        }
        handle_child_exit(pid[1]);
    }
    else {
        if (close(filedes[1]) == -1) {
            print_error("minishell: close", false);
            return ;
        }
        if (dup2(filedes[0], STDIN_FILENO) == -1) {
            print_error("minishell: dup2", false);
            return;
        }
        if (close(filedes[0]) == -1) {
            print_error("minishell: close", false);
            return ;
        }
        execute(node->u_data.binary.right, ht);
        exit(errno);
    }
}

static void    execute_pipe(t_ast_node *node, t_hash_table *ht) {
    int filedes[2];
    pid_t pid[2];

    if (pipe(filedes) == -1) {
        // TODO: Write Normal Error
        print_error("minishell: pipe", false);
        return ;
    }
    pid[0] = fork();
    if (pid[0] == -1) {
        // TODO: write normal error
        print_error("minishell: fork", false);
        return ;
    }
    if (pid[0]) {
        pid[1] = fork();
        if (pid[1] == -1) {
            // TODO: write normal error
            print_error("minishell: fork", false);
            return ;
        }
    }
    execute_pipe_helper(filedes, pid, node, ht);
}

void    execute_binary(t_ast_node *node, t_hash_table *ht) {
    if (node->type == OR_NODE) {
        execute(node->u_data.binary.left, ht);
        if (errno != 0) {
            execute(node->u_data.binary.right, ht);
        }
    }
    else if (node->type == AND_NODE) {
        execute(node->u_data.binary.left, ht);
        if (errno == 0) {
            execute(node->u_data.binary.right, ht);
        }
    }
    else if (node->type == PIPE_NODE)
        execute_pipe(node, ht);
}