/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:04:25 by gsimonia          #+#    #+#             */
/*   Updated: 2025/01/06 18:28:48 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_env(int argc, char **argv, char **envp)
{
	int	i;

	(void)argv;
	if (argc > 1)
		return (write_error(NULL, "ft_env: too many arguments\n", NULL));
	if (!envp || !*envp)
		return (write_error(NULL, "ft_env: environment is empty\n", NULL));
	i = 0;
	while (envp[i])
	{
		write_string(envp[i], "\n", NULL);
		i++;
	}
	return (EXIT_SUCCESS);
}
