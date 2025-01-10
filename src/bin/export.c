/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:06:01 by gsimonia          #+#    #+#             */
/*   Updated: 2025/01/09 10:16:15 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	**create_new_envp(char **envp)
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

static int	set_new_var(char **new_envp, char *key, char *value, int index)
{
	char	*new_var;

	if (!key || !value)
	{
		write_error("export", "key or value is NULL\n", NULL);
		return (EXIT_FAILURE);
	}
	new_var = malloc(ft_strlen(key) + ft_strlen(value) + 2);
	if (!new_var)
		return (write_error("export: ", "memory allocation failed\n", NULL));
	ft_strcpy(new_var, key);
	ft_strcat(new_var, "=");
	ft_strcat(new_var, value);
	new_envp[index] = new_var;
	return (EXIT_SUCCESS);
}

static void	swap_last_two(char **envp)
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

static int	set_env_var(char ***envp, const char *key, const char *value)
{
	int		i;
	int		index;
	char	**new_envp;
	int		k;
	int		new_var_added;

	i = 0;
	index = -1;
	new_var_added = 0;
	new_envp = create_new_envp(*envp);
	if (!new_envp)
		return (EXIT_FAILURE);
	while ((*envp)[i])
	{
		if (ft_strncmp((*envp)[i],
			key, ft_strlen(key)) == 0 && (*envp)[i][ft_strlen(key)] == '=')
		{
			index = i;
			break ;
		}
		i++;
	}
	if (index == -1)
	{
		index = i;
		new_var_added = 1;
	}
	if (set_new_var(new_envp,
			(char *)key, (char *)value, index) != EXIT_SUCCESS)
	{
		k = 0;
		while (k < i)
		{
			free(new_envp[k]);
			k++;
		}
		free(new_envp);
		return (EXIT_FAILURE);
	}
	if (new_var_added)
		swap_last_two(new_envp);
	k = 0;
	while ((*envp)[k])
	{
		free((*envp)[k]);
		k++;
	}
	free(*envp);
	*envp = new_envp;
	return (EXIT_SUCCESS);
}

static void	print_sorted_env(char **envp)
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

int	ft_export(int argc, char **argv, char ***envp)
{
	int		i;
	char	*arg_copy;
	char	*key;
	char	*value;
	char	*save_ptr;

	i = 1;
	if (argc == 1)
	{
		print_sorted_env(*envp);
		return (EXIT_SUCCESS);
	}
	while (argv[i])
	{
		if (!is_valid_env_var_key(argv[i]))
			return (write_error("export", argv[i], "not a valid identifier\n"));
		else if (ft_strchr(argv[i], '=') != NULL)
		{
			arg_copy = ft_strdup(argv[i]);
			if (!arg_copy)
				return (write_error("export",
						"memory allocation failed\n", NULL));
			key = ft_strtok(arg_copy, "=", &save_ptr);
			value = ft_strtok(NULL, "=", &save_ptr);
			if (set_env_var(envp, key, value) != EXIT_SUCCESS)
			{
				free(arg_copy);
				return (EXIT_FAILURE);
			}
			free(arg_copy);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}
