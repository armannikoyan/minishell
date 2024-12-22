/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:04:25 by gsimonia          #+#    #+#             */
/*   Updated: 2024/11/20 16:10:53 by gsimonia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_env(int argc, char **argv, char **envp)
{
	int	i;

	if (argc > 1)
		return (write_error(NULL, "ft_env: too many arguments\n", NULL));
	if (!envp || !*envp)
		return (write_error(NULL, "ft_env: environment is empty\n", NULL));
	i = 0;
	while (envp[i])
	{
		write_string(envp[i], "\n", NULL);
		write(STDOUT_FILENO, "\n", 1);
		i++;
	}
	return (EXIT_SUCCESS);
}
