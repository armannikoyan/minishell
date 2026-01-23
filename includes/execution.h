#ifndef EXECUTION_H
# define EXECUTION_H

#include "ast.h"
#include "hash_table.h"

# define HEREDOC_TMP_FILE "/tmp/minishell_heredoc.tmp"

int  handle_child_exit(pid_t pid);
char **expand_wildcards(char **old_argv);
char    *remove_quotes(char *str);

int    execute_command(t_ast_node *node, t_hash_table *ht, int errnum);
int    execute_binary(t_ast_node *node, t_hash_table *ht, int errnum);
int    execute_redir(t_ast_node *node, t_hash_table *ht, int errnum);

int execute(t_ast_node *node, t_hash_table *ht, int errnum);

#endif