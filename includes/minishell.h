/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasargsy <dasargsy@student.42yerevan.am    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 20:17:20 by anikoyan          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/07/30 16:57:47 by anikoyan         ###   ########.fr       */
=======
/*   Updated: 2024/07/30 02:29:20 by dasargsy         ###   ########.fr       */
>>>>>>> fd0d604047186a539510f33e11f222f7d38a0b8e
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

<<<<<<< HEAD
# define QUOTE_ERROR "Unclosed Quotes"
=======
# include "tokenization.h"
# include "prompt.h"
# include "libft.h"
# include <stdio.h>
# include <history.h>
# include <readline.h>
>>>>>>> fd0d604047186a539510f33e11f222f7d38a0b8e

void	ft_mtx_free(char **mtx);
char	*ft_command_path(char *cmd, char **envp);
char	**ft_format_argv(int argc, char **argv);
char	**ft_get_envp(char **envp);

#endif
