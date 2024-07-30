/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasargsy <dasargsy@student.42yerevan.am    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 20:17:20 by anikoyan          #+#    #+#             */
/*   Updated: 2024/07/30 16:23:21 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define QUOTE_ERROR "Unclosed Quotes"

void	ft_mtx_free(char **mtx);

char	*ft_command_path(char *cmd, char **envp);

char	**ft_format_argv(int argc, char **argv);
char	**ft_get_envp(char **envp);

#endif
