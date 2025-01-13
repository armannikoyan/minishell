/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:07:38 by gsimonia          #+#    #+#             */
/*   Updated: 2025/01/13 19:34:16 by gsimonia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_valid_env_var_key_unset(const char *str)
{
	int	i;

	if (!str || !(*str) || (*str == '='))
		return (0);
	i = 0;
	while (str[i] && str[i] != '=')
	{
		if (!(str[i] == '_' || (str[i] >= 'a' && str[i] <= 'z')
				|| (str[i] >= 'A' && str[i] <= 'Z')
				|| (i > 0 && str[i] >= '0' && str[i] <= '9')))
			return (0);
		i++;
	}
	if (str[i] == '=')
		return (0);
	return (1);
}



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
		if (!is_valid_env_var_key_unset(argv[i]))
		{
			ft_report_error_arg("unset: `", "': not a valid identifier", 1, argv[i]);
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
