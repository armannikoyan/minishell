/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_write.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 23:35:45 by anikoyan          #+#    #+#             */
/*   Updated: 2025/01/14 19:43:43 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	write_error(const char *prefix, const char *message, const char *arg)
{
	if (prefix)
		write(2, prefix, ft_strlen(prefix));
	if (message)
		write(2, message, ft_strlen(message));
	if (arg)
	{
		write(2, arg, ft_strlen(arg));
		write(2, "\n", 1);
	}
	return (EXIT_FAILURE);
}

void	write_string(const char *str1, const char *str2, const char *str3)
{
	if (str1)
		write(1, str1, ft_strlen(str1));
	if (str2)
		write(1, str2, ft_strlen(str2));
	if (str3)
		write(1, str3, ft_strlen(str3));
}

void	print_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		write_string("declare -x ", envp[i], "\n");
		i++;
	}
}

void	print_sorted_env(char **envp)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (envp[i])
	{
		j = i + 1;
		while (envp[j])
		{
			if (envp[i] && envp[j] && ft_strcmp(envp[i], envp[j]) > 0)
			{
				tmp = envp[i];
				envp[i] = envp[j];
				envp[j] = tmp;
			}
			j++;
		}
		i++;
	}
	print_env(envp);
}
