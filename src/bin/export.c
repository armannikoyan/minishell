/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:06:01 by gsimonia          #+#    #+#             */
/*   Updated: 2025/01/06 18:33:50 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	**create_new_envp(char **envp)
{
	int		i;
	char	**new_envp;
	int		j;

	i = 0;
	while (envp[i])
		i++;
	new_envp = malloc(sizeof(char *) * (i + 2));
	if (!new_envp)
		return (NULL);
	j = 0;
	while (j < i)
	{
		new_envp[j] = envp[j];
		j++;
	}
	new_envp[i] = NULL;
	return (new_envp);
}

static int	set_new_var(char **new_envp, char *key, char *value, int index)
{
	char	*new_var;

	new_var = malloc(ft_strlen(key) + ft_strlen(value) + 2);
	if (!new_var)
		return (write_error("export: ", "memory allocation failed\n", NULL));
	ft_strcpy(new_var, key);
	ft_strcat(new_var, "=");
	ft_strcat(new_var, value);
	new_envp[index] = new_var;
	return (EXIT_SUCCESS);
}

static int	set_env_var(char ***envp, const char *key, const char *value)
{
	int		i;
	char	**new_envp;
	int		index;

	new_envp = create_new_envp(*envp);
	if (!new_envp)
		return (EXIT_FAILURE);
	i = 0;
	while ((*envp)[i])
		i++;
	index = i;
	if (set_new_var(new_envp,
			(char *)key, (char *)value, index) != EXIT_SUCCESS)
	{
		free(new_envp);
		return (EXIT_FAILURE);
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
			if (ft_strcmp(envp[i], envp[j]) > 0)
			{
				tmp = envp[i];
				envp[i] = envp[j];
				envp[j] = tmp;
			}
			j++;
		}
		i++;
	}
	i = 0;
	while (envp[i])
		write_string("declare -x ", envp[i++], "\n");
	return ;
}

int	ft_export(int argc, char **argv, char **envp)
{
	int		i;
	char	*key;
	char	*value;

	(void)argc;
	i = 1;
	if (!argv[i])
	{
		print_sorted_env(envp);
		return (EXIT_SUCCESS);
	}
	while (argv[i])
	{
		if (!is_valid_env_var_key(argv[i]))
			return (write_error("ft_export",
					argv[i], "not a valid identifier"));
		else if (ft_strchr(argv[i], '=') != NULL)
		{
			// ISSUE: ft_strtok needs 3 arguments but only 2 are provided
			//
			// key = ft_strtok(argv[i], "=");
			// value = ft_strtok(NULL, "=");
			set_env_var(&envp, key, value);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}
