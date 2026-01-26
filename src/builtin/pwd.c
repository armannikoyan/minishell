/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 01:35:20 by lvarnach          #+#    #+#             */
/*   Updated: 2026/01/27 01:35:47 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <limits.h>

#include "hash_table.h"
#include "utils.h"
#include "builtin.h"
#include "error_codes.h"

// Prints current working directory and finishes with status 0.
// If any arguments passed prints an error and finishes with status 2.
int	ft_pwd(int argc, char **argv, t_hash_table *ht)
{
	char	cwd[PATH_MAX];

	(void) argv;
	(void) ht;
	if (argc > 1 && argv[1][0] == '-')
		return (BUILTIN_ERROR);
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		print_error("pwd: getcwd", false);
		return (BUILTIN_ERROR);
	}
	printf("%s\n", cwd);
	return (0);
}
