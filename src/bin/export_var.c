/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 20:00:27 by gsimonia          #+#    #+#             */
/*   Updated: 2025/01/10 20:00:38 by gsimonia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	find_env_var_index(char **envp, const char *key)
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

static void	free_envp(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

static int	free_partial_envp(char **envp, int env_len)
{
	int	i;

	i = 0;
	while (i < env_len)
	{
		free(envp[i]);
		i++;
	}
	free(envp);
	return (EXIT_FAILURE);
}

static int	update_envp(char ***envp, char **new_envp)
{
	free_envp(*envp);
	*envp = new_envp;
	return (EXIT_SUCCESS);
}

int	set_env_var(char ***envp, const char *key, const char *value)
{
	char	**new_envp;
	int		index;
	int		env_len;
	int		new_var_added;

	env_len = 0;
	new_var_added = 0;
	while ((*envp)[env_len])
		env_len++;
	new_envp = create_new_envp(*envp);
	if (!new_envp)
		return (EXIT_FAILURE);
	index = find_env_var_index(*envp, key);
	if (index == -1)
	{
		index = env_len;
		new_var_added = 1;
	}
	if (set_new_var(new_envp, (char *)key,
			(char *)value, index) != EXIT_SUCCESS)
		return (free_partial_envp(new_envp, env_len));
	if (new_var_added)
		swap_last_two(new_envp);
	return (update_envp(envp, new_envp));
}
