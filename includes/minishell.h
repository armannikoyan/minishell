/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 17:00:14 by anikoyan          #+#    #+#             */
/*   Updated: 2025/01/15 22:49:26 by gsimonia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdbool.h>
# include <stdlib.h>
# include <signal.h>
# include <sys/param.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <dirent.h>
# include <fcntl.h>
# include <string.h>
# include <errno.h>

# include "../libs/libft/libft.h"

# define GREEN "\033[1;32m"
# define WHITE "\033[0m"
# define RED "\x1b[31m"

typedef struct s_token
{
	char			*content;
	char			type;
	unsigned short	subshell_level;
}	t_token;

typedef struct s_node
{
	char			**content;
	char			type;
	unsigned short	subshell_level;
	struct s_node	*left;
	struct s_node	*right;
}	t_node;

typedef struct s_syntax_tree
{
	t_node	*root;
}	t_tree;

typedef struct s_env_context
{
    char            *input;
    char            **envp;
    char            *output;
    unsigned short  i;
    unsigned short  j;
    bool            single_quote;
    bool            double_quote;
}   t_env_context;

void			ft_signal_handler(int sig);
void			ft_parent_child_signal_handler(int sig);
void			ft_child_signal_handler(int sig);
void			ft_heredoc_signal_handler(int sig);
void			ft_error(char *str, int m_errno);
void			ft_tokendelone(void *lst);
void			ft_exec(t_tree *tree, char ***envp);
void			ft_tree_dtor(t_tree **tree);
bool			ft_process_path_patterns(t_list ***lst);
t_tree			*ft_tree_build(t_list **lst);
unsigned short	ft_isoperator(char *c);
unsigned int	ft_envlen(char *input, unsigned int i);
bool			ft_is_valid_env(char *env);
unsigned short	ft_varlen(char *input);
unsigned short	ft_get_quote_count(char *input, unsigned short index);

char			*ft_entry_info(void);
bool			ft_has_syntax_error(t_list **lst);
bool			ft_report_error(const char *prefix,
					const char *content, int error_code);
bool			ft_report_error_arg(const char *prefix, const char *content,
					int error_code, const char *given);
char			*ft_env_expansion(char *input, char **envp);
char			*ft_space_correction(char *input);

t_list			**ft_tokenization(char *input);

int				ft_cd(int argc, char **argv, char **envp);
int				ft_echo(int argc, char **argv);
int				ft_env(int argc, char **argv, char **envp);
int				ft_exit(int argc, char **argv);
int				ft_export(int argc, char **argv, char ***envp);
int				ft_pwd(int argc, char **argv);
int				ft_unset(int argc, char **argv, char **envp);
int				is_valid_env_var_key(const char *str);
int				write_error(const char *prefix,
					const char *message, const char *arg);
int				ft_mtx_strlen(char **mtx);
int				set_new_var(char **new_envp,
					char *key, char *value, int index);
int				set_env_var(char ***envp,
					const char *key, const char *value);
int				handle_sign_and_skip_whitespace(const char *str, int *sign);
int				is_valid_number(const char *str);
int				check_for_overflow(const char *str,
					long long *result, int sign);

long long		ft_long_atoi(const char *str);

char			*ft_strtok(char *str, const char *delimiters, char **save_ptr);
char			**create_new_envp(char **envp);

void			write_string(const char *str1,
					const char *str2, const char *str3);
void			print_env(char **envp);
void			print_sorted_env(char **envp);
void			swap_last_two(char **envp);

DIR				*ft_open_directory(const char *path, struct dirent **entry);
bool			ft_close_directory(DIR *dir);

t_tree			*ft_tree_ctor(void);
void			ft_node_dtor(t_node **node);

t_node			*parse_node(t_list **lst);

pid_t			ft_fork_process(void);

unsigned short	ft_path_len(char *str);

bool			ft_handle_no_matches_found(DIR *dir);
bool			ft_skip_entry(struct dirent *entry, const char *prefix);
bool			ft_is_pattern_match(t_token *token);
bool			ft_identify_command(t_token **token, char **path);
bool			has_single_token_error(t_list **lst);
bool			ft_isbuiltin(t_token *token);
bool			ft_process_pattern(t_list ***lst_ref,
					t_list *current, t_list *prev, t_token *token);
bool			ft_is_matching_pattern(const char *entry_name,
					const char *prefix,
					const char *postfix, bool *matches_found);

void			parse_x_node(t_list **lst, char ***content, char *type_of_node);
void			parse_o_node(t_list **lst, char ***content, char *type_of_node);
void			ft_quote_removal(t_node *node);
void			ft_assign_argument_type(t_list **tmp);
void			ft_handle_argument(t_list **tmp, t_token *token);
void			ft_assign_subshell_levels(t_list **lst);
void			ft_remove_subshell_token(t_list **lst,
					t_list **tmp, t_list *prev);
void			ft_handle_token(t_list **lst,
					t_list **tmp, t_list **prev, int *current_level);
void			ft_entry_info_helper(char **username, char **w_dir);
void			ft_skip_quotes(char *input,
					unsigned short *i, unsigned short *len);
void			ft_copy_quotes(char *output,
					char *input, unsigned short *j, unsigned short *i);
void			handle_operator_len(char *input,
					unsigned short *i, unsigned short *len);
void			handle_operator_copy(char *input,
					char *output, unsigned short *i, unsigned short *j);
void			append_character(char *expanded_input,
					char c, unsigned short *j);
void			ft_fill_content(t_token *token, char *input,
					unsigned short *index, unsigned short len);
void			ft_assign_token_type(t_list ***lst);
void			ft_assign_operator_type(t_list **tmp, t_token *token);
void			ft_assign_command_type(t_token *token, t_list **tmp);
void			ft_create_and_insert_new_node(t_list **lst,
					const char *path, const char *entry_name);

void			ft_exec_with_level(t_node *node,
					char ***envp, unsigned short *current_level);
void			ft_exec_operator(t_node *node,
					char ***envp, unsigned short *current_level);
void			ft_exec_command(t_node *node,
					char ***envp, unsigned short *current_level);
void			handle_fork_error(void);
void			setup_heredoc(t_node *node, const char *temp_file);
void			execute_command(t_node *node, char ***envp);
void			ft_expand_error_code(t_node *node);
void			ft_handle_heredoc(t_node *node, char ***envp);
void			ft_handle_input_redirection(t_node *node, char ***envp);
void			ft_handle_output_redirection(t_node *node,
					char ***envp, int flags);
void			ft_handle_pipe(t_node *node, char ***envp);
void			handle_open_error(void);
void			ft_execute_child_process(t_node *node, char ***envp, int fd);
void			ft_create_pipe(int fd[2]);
void			ft_execute_pipe_child(t_node *node,
					char ***envp, int fd, int redirect_fd);
void			execute_command_in_child(t_node *node, char **envp);
void			handle_heredoc_execution(t_node *node,
					char ***envp, const char *temp_file);
void			child_heredoc_process(t_node *node, const char *temp_file);
void			parent_wait_and_set_status(pid_t pid, int *status);
void			execute_heredoc_child(t_node *node, char ***envp, int fd);
void			ft_execute_input_child_process(t_node *node,
					char ***envp, int fd);

#endif
