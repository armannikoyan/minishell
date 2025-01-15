/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:05:49 by gsimonia          #+#    #+#             */
/*   Updated: 2025/01/15 17:28:31 by gsimonia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_errno;

static long long	ft_long_atoi_check(const char *str)
{
	long long	result;
	int			sign;
	int			start_index;

	start_index = handle_sign_and_skip_whitespace(str, &sign);
	if (check_for_overflow(&str[start_index], &result, sign))
		return (1);
	return (0);
}

int	is_valid_number(const char *str)
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
	if (ft_long_atoi_check(str) == 1)
		return (0);
	return (1);
}

long long	ft_long_atoi(const char *str)
{
	long long	i;
	long long	j;
	long long	min;

	i = 0;
	j = 0;
	min = 1;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\r'
		|| str[i] == '\v' || str[i] == '\f' || str[i] == '\n')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			min = -1;
		i++;
	}
	if (str[i] == '\0')
		return (0);
	while (str[i] >= '0' && str[i] <= '9')
	{
		j *= 10;
		j += (str[i] - '0');
		i++;
	}
	return (j * min);
}

static long long	convert_to_exit_code(const char *str)
{
	long long	exit_code;

	exit_code = ft_long_atoi(str);
	if (exit_code < 0)
		return ((256 + (exit_code % 256)) % 256);
	return (exit_code % 256);
}

int	ft_exit(int argc, char **argv)
{
	write(1, "exit\n", 5);
	if (argc > 1 && !is_valid_number(argv[1]))
	{
		ft_report_error_arg("exit: ",
			": numeric argument required", 255, argv[1]);
		exit(255);
	}
	else if (argc > 2)
	{
		ft_report_error("exit: ", "too many arguments", 1);
		return (1);
	}
	else if (argc == 2)
		g_errno = convert_to_exit_code(argv[1]);
	exit(g_errno);
}
