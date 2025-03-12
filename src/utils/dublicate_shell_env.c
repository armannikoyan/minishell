/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dublicate_shell_env.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 20:09:18 by anikoyan          #+#    #+#             */
/*   Updated: 2025/03/12 20:10:12 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static unsigned short	get_shell_env_row_count(char **envp)
{
	unsigned short	rows;

	rows = 0;
	while (envp[rows])
		++rows;
	return (rows);
}

char	**dup_shell_env(char **envp)
{
	char			**shell_env;
	unsigned short	rows;

	rows = get_shell_env_row_count(envp);
	shell_env = (char **)malloc(sizeof(char *) * (rows + 1));
	if (!shell_env)
	{
		perror("malloc");
		exit(errno);
	}
	rows = 0;
	while (envp[rows])
	{
		shell_env[rows] = ft_strdup(envp[rows]);
		if (!shell_env[rows])
		{
			perror("malloc");
			exit(errno);
		}
		++rows;
	}
	shell_env[rows] = NULL;
	return (shell_env);
}
