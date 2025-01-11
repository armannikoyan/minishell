/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:05:49 by gsimonia          #+#    #+#             */
/*   Updated: 2025/01/11 15:03:24 by gsimonia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_errno;

static void	write_int_to_str(int value, char *str)
{
	int		i;
	int		len;
	char	temp[20];

	i = 0;
	len = 0;
	if (value == 0)
	{
		str[i++] = '0';
		str[i] = '\0';
		return ;
	}
	while (value > 0)
	{
		temp[len] = (value % 10) + '0';
		value /= 10;
		len++;
	}
	while (len > 0)
	{
		str[i] = temp[len - 1];
		i++;
		len--;
	}
	str[i] = '\0';
}

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
	if (str[i] == '0' && str[i + 1] != '\0')
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

static int kill_parent_process(const char *exit_code_str)
{
    int exit_code;
	
	exit_code = ft_atoi(exit_code_str);
    exit(exit_code);
}

int	ft_exit(int argc, char **argv)
{
	long long	exit_code;
	char		exit_code_str[20];

	if (argc > 2)
		return (ft_report_error(NULL, "too many arguments", 1));
	if (argc == 2)
	{
		if (!is_valid_number(argv[1]))
		{
			ft_report_error(NULL, "numeric argument required", 255);
			exit_code = 255;
		}
		else
			exit_code = convert_to_exit_code(argv[1]);
	}
	else
		exit_code = 0;
	g_errno = exit_code;
	write_int_to_str(exit_code, exit_code_str);
	return (kill_parent_process(exit_code_str));
}
