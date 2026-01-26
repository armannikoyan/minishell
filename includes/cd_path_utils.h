/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_path_utils.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 16:56:18 by lvarnach          #+#    #+#             */
/*   Updated: 2026/01/26 16:56:35 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CD_PATH_UTILS_H
# define CD_PATH_UTILS_H
# include "hash_table.h"

# define M1 "CDPATH"

void	set_oldpwd_and_pwd(const char *oldpwd, const char *pwd,
			t_hash_table *ht);

int		check_path(const char *path);

int		try_change_dir(const char *path, t_hash_table *ht, const char *cwd);

char	**split_ev(const char *str, char ***result);
char	*normalize_and_resolve_path(const char *path);

#endif
