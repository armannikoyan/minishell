/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_path_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 19:45:10 by lvarnach          #+#    #+#             */
/*   Updated: 2026/01/28 19:45:10 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

#include "hash_table.h"
#include "utils.h"
#include "../libs/libft/libft.h"

char	*check_path_entry(char *dir, char *cmd)
{
	char	*tmp;
	char	*full;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	full = ft_strjoin(tmp, cmd);
	free(tmp);
	if (!full)
		return (NULL);
	if (access(full, F_OK) == 0)
		return (full);
	free(full);
	return (NULL);
}

char	*search_in_path_env(char *cmd, t_hash_table *ht)
{
	char	**paths;
	char	*res;
	int		i;

	if (!ht_get(ht, "PATH"))
		return (NULL);
	paths = ft_split(ht_get(ht, "PATH")->val, ':');
	if (!paths)
		return (NULL);
	i = 0;
	res = NULL;
	while (paths[i])
	{
		res = check_path_entry(paths[i], cmd);
		if (res)
			break ;
		i++;
	}
	free_split(paths);
	return (res);
}

char	*resolve_cmd_path(char *cmd, t_hash_table *ht)
{
	char	*path;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path = search_in_path_env(cmd, ht);
	if (!path)
		return (ft_strdup(cmd));
	return (path);
}
