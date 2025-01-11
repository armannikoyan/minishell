/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:06:01 by gsimonia          #+#    #+#             */
/*   Updated: 2025/01/11 14:08:49 by gsimonia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**create_new_envp(char **envp)
{
	int		i;
	char	**new_envp;

	i = 0;
	while (envp[i])
		i++;
	new_envp = malloc(sizeof(char *) * (i + 2));
	if (!new_envp)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		new_envp[i] = ft_strdup(envp[i]);
		if (!new_envp[i])
		{
			while (--i >= 0)
				free(new_envp[i]);
			free(new_envp);
			return (NULL);
		}
		i++;
	}
	new_envp[i] = NULL;
	return (new_envp);
}

int	set_new_var(char **new_envp, char *key, char *value, int index)
{
	char	*new_var;

	if (!key || !value)
	{
		return (ft_report_error("export", "key or value is NULL", 1));
	}
	new_var = malloc(ft_strlen(key) + ft_strlen(value) + 2);
	if (!new_var)
		return (ft_report_error("export", "memory allocation failed", 1));
	ft_strcpy(new_var, key);
	ft_strcat(new_var, "=");
	ft_strcat(new_var, value);
	new_envp[index] = new_var;
	return (EXIT_SUCCESS);
}

void	swap_last_two(char **envp)
{
	int		i;
	int		last_index;
	int		second_last_index;
	char	*tmp;

	i = 0;
	last_index = -1;
	second_last_index = -1;
	while (envp[i])
	{
		if (envp[i + 1] == NULL)
			last_index = i;
		else if (envp[i + 2] == NULL)
			second_last_index = i;
		i++;
	}
	if (last_index != -1 && second_last_index != -1)
	{
		tmp = envp[last_index];
		envp[last_index] = envp[second_last_index];
		envp[second_last_index] = tmp;
	}
}

int	handle_export_argument(char *arg, char ***envp)
{
	char	*arg_copy;
	char	*key;
	char	*value;
	char	*save_ptr;

	arg_copy = ft_strdup(arg);
	if (!arg_copy)
		return (ft_report_error("export", "memory allocation failed", 1));
	key = ft_strtok(arg_copy, "=", &save_ptr);
	value = ft_strtok(NULL, "=", &save_ptr);
	if (set_env_var(envp, key, value) != EXIT_SUCCESS)
	{
		free(arg_copy);
		return (EXIT_FAILURE);
	}
	free(arg_copy);
	return (EXIT_SUCCESS);
}

int	ft_export(int argc, char **argv, char ***envp)
{
	int	i;

	i = 1;
	if (argc == 1)
	{
		print_sorted_env(*envp);
		return (EXIT_SUCCESS);
	}
	while (argv[i])
	{
		if (!is_valid_env_var_key(argv[i]))
			return (ft_report_error(argv[i], " not a valid identifier", 1));
		else if (ft_strchr(argv[i], '=') != NULL)
		{
			if (handle_export_argument(argv[i], envp) != EXIT_SUCCESS)
				return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}
