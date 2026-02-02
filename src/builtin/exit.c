/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 01:16:11 by lvarnach          #+#    #+#             */
/*   Updated: 2026/01/31 01:50:46 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>

#include "ast.h"
#include "hash_table.h"
#include "builtin.h"
#include "error_codes.h"
#include "utils.h"
#include "../../libs/libft/libft.h"

static int	get_error_status(const char *s)
{
	int	len;

	len = 0;
	if (s[len] == '+' || s[len] == '-')
		++len;
	while (s[len])
	{
		if (!isdigit(s[len]))
		{
			(print_error("exit: ", true), print_error(s, true));
			print_error(": numeric argument required\n", true);
			return (-1);
		}
		++len;
	}
	return (ft_atoi(s) & 0xFF);
}

// Terminates the program with a status.
// If status passed as an argument checks is all the symbols
// are digits and is last three of them in 0-255 interval.
// If passed more than 1 argument or status not in interval
// terminates a program with status 2.
// If no arguments passed terminates a program with
// status of the last command executed (takes status from errno)
int	ft_exit(char **argv, t_garbage *g, int errnum)
{
	int	argc;

	argc = 0;
	while (argv[argc])
		argc++;
	if (argc == 2)
	{
		if (argv[1][0] == '\0')
		{
			print_error("exit: numeric argument required\n", true);
			return (BUILTIN_ERROR);
		}
		errnum = get_error_status(argv[1]);
		if (errnum < 0)
			return (BUILTIN_ERROR);
	}
	if (argc == 1 || argc == 2)
	{
		if (g){
			clean_all_stacks(g);
			ast_deletion(g->root);
			ht_destroy(g->ht);
		}
		rl_clear_history();
		exit(errnum);
	}
	print_error("exit: too many arguments\n", true);
	return (BUILTIN_ERROR);
}
