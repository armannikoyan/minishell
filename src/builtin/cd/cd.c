/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 01:49:28 by lvarnach          #+#    #+#             */
/*   Updated: 2026/01/27 02:13:26 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

#include "cd_path_utils.h"
#include "path_utils.h"
#include "utils.h"
#include "builtin.h"
#include "error_codes.h"

static char	*get_path(const char *cd_path, const char *path)
{
	char	cwd[PATH_MAX];
	char	*full;
	char	*tmp;

	full = concat_path(cd_path, path);
	if (cd_path[0] != '/')
	{
		if (getcwd(cwd, sizeof(cwd)) == NULL)
		{
			print_error("cd: change_dir: get_path_comb: getcwd", false);
			return (NULL);
		}
		tmp = concat_path(cwd, full);
		free(full);
		full = tmp;
	}
	tmp = normalize_and_resolve_path(full);
	free(full);
	return (tmp);
}

int	resolve_cd_path(int i, char **cd_path, const char *path, t_hash_table *ht)
{
	char	cwd[PATH_MAX];
	char	*res;

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		print_error("cd: change_dir: get_path_comb: getcwd", false);
		return (BUILTIN_ERROR);
	}
	while (cd_path[i])
	{
		res = get_path(cd_path[i], path);
		if (res != NULL && chdir(res) == 0)
		{
			set_oldpwd_and_pwd(cwd, res, ht);
			return (printf("%s\n", res), free(res), free_split(cd_path), 0);
		}
		free(res);
		i++;
	}
	free_split(cd_path);
	print_error("cd: no path found\n", true);
	return (BUILTIN_ERROR);
}

static int	analyse_path(char **argv, t_hash_table *ht)
{
	int		i;
	char	cwd[PATH_MAX];
	char	**cd;
	char	*target;
	char	*tmp;

	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (print_error("cd: getcwd", false), BUILTIN_ERROR);
	if (argv[1][0] == '/')
	{
		target = normalize_and_resolve_path(argv[1]);
		i = try_change_dir(target, ht, cwd);
		free(target);
		return (i);
	}
	i = 0;
	if (ht_get(ht, M1) == NULL || split_ev(ht_get(ht, M1)->val, &cd) == NULL)
	{
		target = concat_path(cwd, argv[1]);
		tmp = normalize_and_resolve_path(target);
		free(target);
		i = try_change_dir(tmp, ht, cwd);
		return (free(tmp), i);
	}
	return (resolve_cd_path(i, cd, argv[1], ht));
}

static int	solve_env_var(char *target, bool should_print, t_hash_table *ht)
{
	int		res;
	char	cwd[PATH_MAX];
	char	*tmp;

	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (print_error("cd: getcwd", false), BUILTIN_ERROR);
	if (target[0] != '/')
	{
		target = concat_path(cwd, target);
		tmp = normalize_and_resolve_path(target);
		free(target);
		res = try_change_dir(tmp, ht, cwd);
		return (free(tmp), res);
	}
	tmp = normalize_and_resolve_path(target);
	res = try_change_dir(tmp, ht, cwd);
	if (res == 0 && should_print == true)
		printf("%s\n", tmp);
	return (free(tmp), res);
}

int	ft_cd(int argc, char **argv, t_hash_table *ht)
{
	char	*target;

	if (argc == 1)
	{
		if (ht_get(ht, "HOME") == NULL)
			return (print_error("cd: 'HOME' doesn't exist\n", true), 2);
		target = ht_get(ht, "HOME")->val;
		if (target == NULL)
			return (print_error("cd: 'HOME' is empty\n", true), 2);
		return (solve_env_var(target, false, ht));
	}
	if (argc == 2 && argv[1][0] != '\0')
	{
		if (strcmp("-", argv[1]) == 0)
		{
			if (ht_get(ht, "OLDPWD") == NULL)
				return (print_error("cd: 'OLDPWD' doesn't exist\n", true), 2);
			target = ht_get(ht, "OLDPWD")->val;
			if (target == NULL)
				return (print_error("cd: 'OLDPWD' is empty\n", true), 2);
			return (solve_env_var(target, true, ht));
		}
		return (analyse_path(argv, ht));
	}
	return (print_error("cd: too many arguments", true), BUILTIN_ERROR);
}
