/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insert_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 19:05:40 by lvarnach          #+#    #+#             */
/*   Updated: 2026/01/26 19:06:07 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "hash_table.h"
#include "utils.h"
#include "../libs/libft/libft.h"

static void	update_shlvl(t_hash_table *ht)
{
	char	*shlvl;
	char	*str;

	if (ht_get(ht, "SHLVL") == NULL)
	{
		if (ht_create_bucket(ht, "SHLVL", NULL, false) == -1)
			exit(EXIT_FAILURE);
	}
	shlvl = ht_get(ht, "SHLVL")->val;
	if (shlvl)
		str = ft_itoa(ft_atoi(shlvl) + 1);
	else
		str = ft_itoa(1);
	if (!str)
	{
		print_error("minishell: Failed to duplicate environment.\n", true);
		exit(EXIT_FAILURE);
	}
	ht_update_value(ht, "SHLVL", str);
	free(str);
}

static void	replace_incorrect_env(t_hash_table *ht)
{
	char	str[PATH_MAX];

	update_shlvl(ht);
	if (getcwd(str, sizeof(str)) == NULL)
		print_error("minishell: replace_incorrect_env: getcwd", false);
	if (ht_get(ht, "PWD") == NULL)
		ht_create_bucket(ht, "PWD", str, false);
	else
		ht_update_value(ht, "PWD", str);
	if (ht_get(ht, "OLDPWD") == NULL)
		ht_create_bucket(ht, "OLDPWD", NULL, false);
}

static size_t	max_env_len(char **envp)
{
	size_t	max_len;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	max_len = 0;
	while (envp[++i])
	{
		j = ft_strchr(envp[i], '=') - envp[i];
		if (j > max_len)
			max_len = j;
	}
	return (max_len);
}

void	insert_env(t_hash_table *ht, char **envp)
{
	char		*str;
	size_t		i;
	size_t		j;

	i = -1;
	str = (char *)ft_calloc(max_env_len(envp) + 1, sizeof(char));
	if (!str)
	{
		print_error("minishell: insert_env: malloc", false);
		exit(EXIT_FAILURE);
	}
	i = -1;
	while (envp[++i])
	{
		j = ft_strchr(envp[i], '=') - envp[i];
		ft_strlcpy(str, envp[i], j + 1);
		ht_create_bucket(ht, str, &envp[i][j + 1], false);
	}
	free(str);
	replace_incorrect_env(ht);
}
