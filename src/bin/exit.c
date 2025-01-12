/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:05:49 by gsimonia          #+#    #+#             */
/*   Updated: 2025/01/12 07:36:37 by gsimonia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_errno;

static int	is_valid_number(const char *str)
{
	int	i;

	i = 0;
	if (!str || *str == '\0')
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (str[i] == '\0')
		return (0);
	while (str[i] != '\0')
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static long long	convert_to_exit_code(const char *str)
{
	long long	exit_code;

	exit_code = ft_long_atoi(str);
	if (exit_code < 0)
		return ((256 + (exit_code % 256)) % 256);
	return (exit_code % 256);
}

static int	kill_parent_process(int exit_code)
{
	exit(exit_code);
}

int	ft_exit(int argc, char **argv)
{
	long long	exit_code;

	if (argc > 2)
	{
		if (is_valid_number(argv[1]))
			exit_code = 1;
		else
			exit_code = 255;
		ft_report_error("exit:", " too many arguments", exit_code);
	}
	else if (argc == 2)
	{
		if (!is_valid_number(argv[1]))
		{
			ft_report_error_arg("exit: ",
				": numeric argument required", 255, argv[1]);
			exit_code = 255;
		}
		else
			exit_code = convert_to_exit_code(argv[1]);
	}
	else
		exit_code = 0;
	g_errno = exit_code;
	return (kill_parent_process(exit_code));
}
