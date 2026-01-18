#ifndef EXECUTION_H
# define EXECUTION_H

#include "ast.h"
#include "hash_table.h"

# define HEREDOC_TMP_FILE "/tmp/minishell_heredoc.tmp"

void    handle_child_exit(pid_t pid);

void    execute_command(t_ast_node *node, t_hash_table *ht);
void    execute_binary(t_ast_node *node, t_hash_table *ht);
void    execute_redir(t_ast_node *node, t_hash_table *ht);

void    execute(t_ast_node *node, t_hash_table *ht);

#endif