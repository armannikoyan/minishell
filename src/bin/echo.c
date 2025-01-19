/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:03:48 by gsimonia          #+#    #+#             */
/*   Updated: 2025/01/19 16:59:55 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_echo(int argc, char **argv)
{
	int		i;
	bool	n_flag;

	i = 1;
	n_flag = false;
	while (i < argc && argv[i] && argv[i][0] == '-' && ft_strchr(argv[i], 'n'))
	{
		n_flag = true;
		i++;
	}
	while (i < argc && argv[i])
	{
		write(STDOUT_FILENO, argv[i], ft_strlen(argv[i]));
		if (i + 1 < argc)
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (!n_flag)
		write(STDOUT_FILENO, "\n", 1);
	return (EXIT_SUCCESS);
}
