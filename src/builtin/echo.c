/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 01:43:36 by lvarnach          #+#    #+#             */
/*   Updated: 2026/01/27 01:44:35 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdio.h>

#include "hash_table.h"
#include "builtin.h"

static bool	is_n_flag(char *s)
{
	int	i;

	if (s == NULL || s[0] == '\0')
		return (false);
	if (s[0] != '-' || s[1] != 'n')
		return (false);
	i = 2;
	while (s[i] && s[i] == 'n')
		i++;
	if (s[i] != '\0')
		return (false);
	return (true);
}

// Prints all passed arguments and finishes with status 0.
// In case of option '-n' passed doesn't add '\n' at the end.
int	ft_echo(int argc, char **argv, t_hash_table *ht)
{
	int		i;
	bool	n_flag;
	bool	is_first_print;

	(void) ht;
	i = 1;
	n_flag = false;
	if (is_n_flag(argv[i]))
	{
		n_flag = true;
		i++;
	}
	is_first_print = false;
	while (i < argc)
	{
		if (is_n_flag(argv[i++]) && !is_first_print)
			continue ;
		is_first_print = true;
		printf("%s", argv[--i]);
		if (i++ != argc - 1)
			printf(" ");
	}
	if (!n_flag)
		printf("\n");
	return (0);
}
