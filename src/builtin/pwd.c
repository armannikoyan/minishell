/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 01:35:20 by lvarnach          #+#    #+#             */
/*   Updated: 2026/02/02 22:39:51 by lvarnach         ###   ########.fr       */
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
	t_entry	*entry;

	if (argc > 1 && argv[1][0] == '-')
		return (BUILTIN_ERROR);
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		entry = ht_get(ht, "PWD");
		if (entry == NULL || entry->val == NULL)
		{
			print_error("pwd: error retrieving current directory\n", true);
			return (BUILTIN_ERROR);
		}
		printf("%s\n", entry->val);
		return (0);
	}
	printf("%s\n", cwd);
	return (0);
}
