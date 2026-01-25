#ifndef EXECUTION_H
# define EXECUTION_H

#include <sys/types.h>

#include "ast.h"
#include "hash_table.h"

# define HEREDOC_TMP_FILE "/tmp/minishell_heredoc.tmp"

int  handle_child_exit(pid_t pid);
char **expand_wildcards(char **old_argv);
char    *remove_quotes(char *str);

int    execute_command(t_ast_node *node, t_hash_table *ht, int errnum);
int execute_pipeline(t_ast_node *node, t_hash_table *ht, int errnum);
int setup_redirection(t_ast_node *node, t_hash_table *ht, int *saved_fd, int *target_fd, int errnum);
int cleanup_redirection(t_ast_node *node, int saved_fd, int target_fd);
int scan_and_process_heredocs(t_ast_node *node, t_hash_table *ht, int *counter);
void cleanup_heredoc_files(int count);

int execute(t_ast_node *node, t_hash_table *ht, int errnum);

#endif