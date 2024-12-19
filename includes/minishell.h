/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 17:00:14 by anikoyan          #+#    #+#             */
/*   Updated: 2024/12/09 13:35:58 by anikoyan         ###   ########.fr       */
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

# include "../libs/libft/libft.h"

# define GREEN "\033[1;32m"
# define WHITE "\033[0m"
# define RED "\x1b[31m"

typedef struct s_token
{
	char	*content;
	char	type;
	bool	subshell;
}	t_token;

typedef struct s_node
{
	char			**content;
	char			type;
	struct s_node	*left;
	struct s_node	*right;
}	t_node;

typedef struct s_syntax_tree
{
	t_node	*root;
}	t_tree;

void			ft_signal_handler(int signum);
void			ft_error(char **str, int m_errno);
void			ft_tokendelone(void *lst);
t_tree			*ft_tree_build(t_list **lst);
unsigned short	ft_isoperator(char *c);
unsigned int	ft_envlen(char *input, unsigned int i);
bool			ft_is_valid_env(char *env);
unsigned short	ft_varlen(char *input);
unsigned short	ft_get_quote_count(char *input, unsigned short index);

char			*ft_entry_info(void);
bool			ft_has_syntax_error(t_list **lst);
char			*ft_env_expansion(char *input, char **envp);
char			*ft_space_correction(char *input);

t_list			**ft_tokenization(char *input);

unsigned short	copy_corrected_output(char *output, char *input);
bool			is_special_character(char c);
void			handle_parentheses_length(char *input,
					unsigned short *i, unsigned short *len);
unsigned short	handle_general_characters(char *input,
					unsigned short *i, unsigned short *len);
void			handle_spaces(char *input,
					unsigned short *i, unsigned short *len);
void			skip_leading_spaces(char *input, unsigned short *i);
unsigned short	calculate_corrected_length(char *input);
void			handle_space_after_parentheses(char *output,
					char *input, unsigned short *i, unsigned short *j);
t_list			**tokenize_and_build_tree(char *input, t_tree **tree);
char			*process_input(char *input, char **envp);
void			process_valid_input(char *input, char **envp);
void			handle_input(char *prompt, char **envp);
void			free_resources(char *input, t_list **lst, t_tree *tree);
void			clean_exit(char *prompt);
void			handle_operator_token(t_list **tmp);
void			copy_token_content(char *dest,
					const char *input, unsigned short *index);
unsigned short	calculate_token_length(const char *input,
					unsigned short *index);
char			*join_paths(const char *base, const char *suffix);
void			set_command_type(t_token *token);
int				operator_has_higher_precedence(t_node *new_op,
					t_node *current_op);
int				operator_precedence(char *op);

int				is_valid_env_var_key(const char *str);
char			*ft_strtok(char *str, const char *delimiters, char **save_ptr);
long long		ft_long_atoi(const char *str);
int				write_error(const char *prefix,
					const char *message, const char *arg);
void			write_string(const char *str1,
					const char *str2, const char *str3);
int				ft_cd(int argc, char **argv, char **envp);
int				ft_echo(int argc, char **argv);
int				ft_env(int argc, char **argv, char **envp);
int				ft_exit(int argc, char **argv);
int				ft_export(int argc, char **argv, char **envp);
int				ft_pwd(int argc, char **argv);
int				ft_unset(int argc, char **argv, char **envp);

#endif
