/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:03:48 by gsimonia          #+#    #+#             */
/*   Updated: 2025/01/07 22:23:38 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_echo(int argc, char **argv)
{
	int		i;
	bool	n_flag;

	i = 1;
	n_flag = false;
	if (i < argc && argv[i] && ft_strcmp(argv[i], "-n") == 0)
	{
		n_flag = true;
		i++;
	}
	while (i < argc && argv[i])
	{
		if (write(STDOUT_FILENO, argv[i], ft_strlen(argv[i])) == -1)
			return (EXIT_FAILURE);
		if (i + 1 < argc)
		{
			if (write(STDOUT_FILENO, " ", 1) == -1)
				return (EXIT_FAILURE);
		}
		i++;
	}
	if (!n_flag)
		if (write(STDOUT_FILENO, "\n", 1) == -1)
			return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
