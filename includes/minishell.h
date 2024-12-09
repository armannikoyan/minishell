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
	char	type;
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

#endif
