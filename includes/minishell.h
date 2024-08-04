/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasargsy <dasargsy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 12:52:21 by dasargsy          #+#    #+#             */
/*   Updated: 2024/08/04 17:35:38 by dasargsy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "prompt.h"
# include "../libs/libft/libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "gnl.h"

void	ft_mtx_free(char **mtx);
char	*ft_command_path(char *cmd, char **envp);
char	**ft_format_argv(int argc, char **argv);
char	**ft_get_envp(char **envp);

#endif
