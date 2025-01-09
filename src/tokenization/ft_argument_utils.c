/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_argument_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 16:22:48 by gsimonia          #+#    #+#             */
/*   Updated: 2025/01/09 16:22:49 by gsimonia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

DIR	*ft_open_directory(const char *path, struct dirent **entry)
{
	DIR	*dir;

	if (ft_strcmp(path, "") == 0)
		dir = opendir("./");
	else
		dir = opendir(path);
	if (!dir)
	{
		ft_report_error("no matches found: ", path, 1);
		return (NULL);
	}
	*entry = readdir(dir);
	if (!*entry)
	{
		ft_report_error("no matches found: ", path, 1);
		closedir(dir);
		return (NULL);
	}
	return (dir);
}

bool	ft_is_matching_pattern(const char *entry_name,
		const char *prefix, const char *postfix, bool *matches_found)
{
	if ((ft_strncmp(entry_name, prefix, ft_strlen(prefix)) == 0)
		&& (ft_strlen(entry_name)
			>= ft_strlen(prefix) + ft_strlen(postfix))
		&& (ft_strcmp(entry_name + ft_strlen(entry_name)
				- ft_strlen(postfix), postfix) == 0))
	{
		*matches_found = true;
		return (true);
	}
	return (false);
}

bool	ft_handle_no_matches_found(t_list *lst, DIR *dir)
{
	ft_report_error("no matches found: ",
		((t_token *)lst->content)->content, 1);
	ft_close_directory(dir);
	return (false);
}

bool	ft_skip_entry(struct dirent *entry, const char *prefix)
{
	if ((entry->d_name[0] == '.' && ((ft_strlen(entry->d_name) == 1)
				|| (ft_strlen(entry->d_name) == 2
					&& entry->d_name[1] == '.')))
		|| (ft_strncmp(prefix, ".", 1) != 0
			&& ft_strncmp(entry->d_name, ".", 1) == 0))
		return (true);
	return (false);
}
