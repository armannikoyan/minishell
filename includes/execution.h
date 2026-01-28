/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 16:32:47 by lvarnach          #+#    #+#             */
/*   Updated: 2026/01/28 02:22:31 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include <sys/types.h>

# include "ast.h"
# include "hash_table.h"
# include "../libs/libft/libft.h"

# define HEREDOC_TMP_FILE "/tmp/minishell_heredoc.tmp"

typedef struct s_exec_frame
{
	t_ast_node	*node;
	int			state;
	int			saved_fd;
	int			target_fd;
}	t_exec_frame;

typedef struct s_exec_ctx
{
	t_list			**stack;
	t_hash_table	*ht;
	int				*status;
}	t_exec_ctx;

typedef struct s_p_ctx
{
	t_hash_table	*ht;
	int				errnum;
	int				prev_fd;
	pid_t			last_pid;
}	t_p_ctx;

void	cleanup_heredoc_files(int count);
int		handle_child_exit(pid_t pid);
int		execute_command(t_ast_node *node, t_hash_table *ht, int errnum);
int		execute_pipeline(t_ast_node *node, t_hash_table *ht, int errnum);
int		setup_redirection(t_ast_node *node, t_hash_table *ht, int *save,
			int *tgt);
int		execute_subshell(t_ast_node *node, t_hash_table *ht, int errnum);
int		cleanup_redirection(t_ast_node *node, int saved_fd, int target_fd);
int		scan_and_process_heredocs(t_ast_node *node, t_hash_table *ht,
			int *counter);
int		apply_dup2(int fd, int target_fd, int *saved_fd);
int		prepare_filename(t_ast_node *node, int *saved_fd);
int		setup_file_redir(t_ast_node *node, int flags, int std_fd, int *save);
int		handle_heredoc_line(char *line, t_doc_ctx *ctx);
int		process_heredoc_loop(t_doc_ctx *ctx);
void	prepare_limiter(t_ast_node *node, t_doc_ctx *ctx);
int		setup_heredoc(t_ast_node *node, t_hash_table *ht, int *s_fd, int e);
void	handle_state_zero(t_exec_frame *curr, t_exec_ctx *d);
void	handle_state_one(t_exec_frame *curr, t_exec_ctx *d);
int		push_new_frame(t_list **stack, t_ast_node *node);
void	pop_frame(t_list **stack);
void	update_underscore(t_hash_table *ht, char *value);
char	**ht_to_envp(t_hash_table *ht);
char	*resolve_cmd_path(char *cmd, t_hash_table *ht);
char	*check_path_entry(char *dir, char *cmd);
char	*search_in_path_env(char *cmd, t_hash_table *ht);
int		expand_argv_vars(t_ast_node *cmd, t_hash_table *ht, int errnum);
int		run_expansion(t_ast_node *node, t_hash_table *ht, int errnum);
int		check_basic_builtins(char *cmd, char **av, t_hash_table *ht);
int		check_extra_builtins(char *cmd, char **av, t_hash_table *ht, int e);
int		run_builtin(char **argv, t_hash_table *ht, int errnum);
void	handle_redir_init(t_exec_frame *frame, t_exec_ctx *ctx);
int		execute(t_ast_node *node, t_hash_table *ht, int errnum);
char	**expand_wildcards(char **old_argv);
char	*remove_quotes(char *str);

#endif
