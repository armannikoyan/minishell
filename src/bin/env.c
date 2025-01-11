/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:04:25 by gsimonia          #+#    #+#             */
/*   Updated: 2025/01/11 11:31:22 by gsimonia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_env(int argc, char **argv, char **envp)
{
	int	i;

	(void)argv;
	if (argc > 1)
		return (ft_report_error("env: ", "too many arguments", 128));
	if (!envp || !*envp)
		return (ft_report_error("env: ", "environment is empty", 129));
	i = 0;
	while (envp[i])
	{
		write_string(envp[i], "\n", NULL);
		i++;
	}
	return (EXIT_SUCCESS);
}
