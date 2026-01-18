#include <stdlib.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <stdio.h>
#include <readline/readline.h>

#include "ast.h"
#include "execution.h"
#include "expansion.h"
#include "hash_table.h"
#include "tokenization.h"
#include "utils.h"
#include "../../libs/libft/libft.h"

static void    execute_redir_helper(t_ast_node *node, t_hash_table *ht, int target_fd, int open_flags) {
    int original_std_fileno;
    char *tmp;

    original_std_fileno = dup(target_fd);
    if (original_std_fileno == -1) {
        // TODO: Write normal error
        print_error("minishell: dup", false);
        return;
    }
    tmp = node->u_data.redir.filename;
    node->u_data.redir.filename = remove_quotes(tmp);
    if (node->u_data.redir.filename == NULL) {
        print_error("minishell: malloc", false);
        return ;
    }
    free(tmp);
    node->u_data.redir.fd = open(node->u_data.redir.filename, open_flags, 0644);
    if (node->u_data.redir.fd == -1) {
        // TODO: Write normal error
        print_error("minishell: open", false);
        if (close(original_std_fileno) == -1) {
            // TODO: Write normal error
            print_error("minishell: close", false);
        }
        return;
    }
    if (dup2(node->u_data.redir.fd, target_fd) == -1) {
        print_error("minishell: dup2", false);
        return;
    }
    if (close(node->u_data.redir.fd) == -1) {
        print_error("minishell: close", false);
        return;
    }
    execute(node->u_data.redir.child, ht);
    if (dup2(original_std_fileno, target_fd) == -1) {
        print_error("minishell: dup2", false);
        return;
    }
    if (close(original_std_fileno) == -1)
        print_error("minishell: close", false);
}

void    execute_heredoc(t_ast_node *node, t_hash_table *ht) {
    int original_stdin_fileno;
    char *line;
    char *tmp;
    char    quote_char;
    size_t  i;

    original_stdin_fileno = dup(STDIN_FILENO);
    if (original_stdin_fileno == -1) {
        // TODO: Write normal error
        print_error("minishell: dup", false);
        return;
    }
    node->u_data.redir.fd = open(HEREDOC_TMP_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (node->u_data.redir.fd == -1) {
        // TODO: Write normal error
        print_error("minishell: open", false);
        if (close(original_stdin_fileno) == -1) {
            // TODO: Write normal error
            print_error("minishell: close", false);
        }
        return;
    }
    quote_char = 0;
    i = -1;
    while (node->u_data.redir.filename[++i]) {
        set_quote_char(node->u_data.redir.filename[i], &quote_char);
        if (quote_char)
            break ;
    }
    tmp = node->u_data.redir.filename;
    node->u_data.redir.filename = remove_quotes(tmp);
    if (node->u_data.redir.filename == NULL) {
        print_error("minishell: malloc", false);
        return ;
    }
    free(tmp);
    while (true)
    {
        line = readline("> ");
        if (!line || ft_strcmp(line, node->u_data.redir.filename) == 0)
        {
            free(line);
            break ;
        }
        if (!quote_char) {
            tmp = line;
            line = expand_dollar_sign(tmp, ht);
            free(tmp);
        }
        if (write(node->u_data.redir.fd, line, ft_strlen(line)) == -1) {
            free(line);
            if (close(node->u_data.redir.fd) == -1) {
                print_error("minishell: close", false);
                return;
            }
            print_error("minishell: write", false);
            return;
        }
        if (write(node->u_data.redir.fd, "\n", 1) == -1)
        {
            free(line);
            if (close(node->u_data.redir.fd) == -1) {
                print_error("minishell: close", false);
                return;
            }
            print_error("minishell: write", false);
            return;
        }
        free(line);
    }
    if (close(node->u_data.redir.fd) == -1) {
        print_error("minishell: close", false);
        return;
    }
    node->u_data.redir.fd = open(HEREDOC_TMP_FILE, O_RDONLY, 0644);
    if (node->u_data.redir.fd == -1) {
        // TODO: Write normal error
        print_error("minishell: open", false);
        if (close(original_stdin_fileno) == -1) {
            // TODO: Write normal error
            print_error("minishell: close", false);
        }
        return;
    }
    if (dup2(node->u_data.redir.fd, STDIN_FILENO) == -1) {
        print_error("minishell: dup2", false);
        return;
    }
    if (close(node->u_data.redir.fd) == -1) {
        print_error("minishell: close", false);
        return;
    }
    execute(node->u_data.redir.child, ht);
    if (unlink(HEREDOC_TMP_FILE) == -1) {
        print_error("minishell: unlink", false);
    }
    if (dup2(original_stdin_fileno, STDIN_FILENO) == -1) {
        print_error("minishell: dup2", false);
        return;
    }
    if (close(original_stdin_fileno) == -1)
        print_error("minishell: close", false);
}

void    execute_redir(t_ast_node *node, t_hash_table *ht) {
    if (node->type == REDIRECT_IN_NODE)
        execute_redir_helper(node, ht, STDIN_FILENO, O_RDONLY);
    else if (node->type == REDIRECT_OUT_NODE)
        execute_redir_helper(node, ht, STDOUT_FILENO, O_WRONLY | O_CREAT | O_TRUNC);
    else if (node->type == REDIRECT_APPEND_NODE)
        execute_redir_helper(node, ht, STDOUT_FILENO, O_WRONLY | O_CREAT | O_APPEND);
    else if (node->type == HEREDOC_NODE)
        execute_heredoc(node, ht);
}