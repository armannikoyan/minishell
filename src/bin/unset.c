/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:07:38 by gsimonia          #+#    #+#             */
/*   Updated: 2024/11/20 14:56:15 by gsimonia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	get_env_var_index(char **envp, const char *key)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, ft_strlen(key)) == 0
			&& envp[i][ft_strlen(key)] == '=')
			return (i);
		i++;
	}
	return (-1);
}

static void	remove_env_var(char **envp, int idx)
{
	if (!envp || idx < 0)
		return ;
	while (envp[idx])
	{
		envp[idx] = envp[idx + 1];
		idx++;
	}
}

int	ft_unset(int argc, char **argv, char **envp)
{
	int	i;
	int	idx;
	int	exit_status;

	i = 1;
	exit_status = EXIT_SUCCESS;
	if (argc == 1)
		return (EXIT_SUCCESS);
	while (i < argc)
	{
		if (!is_valid_env_var_key(argv[i]))
		{
			write_error("unset: ", "`", argv[i]);
			exit_status = EXIT_FAILURE;
		}
		else
		{
			idx = get_env_var_index(envp, argv[i]);
			if (idx != -1)
				remove_env_var(envp, idx);
		}
		i++;
	}
	return (exit_status);
}
