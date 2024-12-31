/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 17:00:14 by anikoyan          #+#    #+#             */
/*   Updated: 2024/12/31 22:30:19 by anikoyan         ###   ########.fr       */
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
# include <dirent.h>
# include <fcntl.h>

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

void			ft_signal_handler(int signum);
void			ft_error(char *str, int m_errno);
void			ft_tokendelone(void *lst);
void			ft_exec(t_tree *tree, char **envp);
void			ft_tree_dtor(t_tree **tree);
bool			ft_process_path_patterns(t_list **lst);
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
char			*ft_env_expansion(char *input, char **envp);
char			*ft_space_correction(char *input);

t_list			**ft_tokenization(char *input);

int				ft_cd(int argc, char **argv, char **envp);
int				ft_echo(int argc, char **argv);
int				ft_env(int argc, char **argv, char **envp);
int				ft_exit(int argc, char **argv);
int				ft_export(int argc, char **argv, char **envp);
int				ft_pwd(int argc, char **argv);
int				ft_unset(int argc, char **argv, char **envp);
int				is_valid_env_var_key(const char *str);
int				write_error(const char *prefix,
					const char *message, const char *arg);

long long		ft_long_atoi(const char *str);

char			*ft_strtok(char *str, const char *delimiters, char **save_ptr);

void			write_string(const char *str1,
					const char *str2, const char *str3);

DIR				*ft_open_directory(const char *path, struct dirent **entry);
bool			ft_close_directory(DIR *dir);

#endif
