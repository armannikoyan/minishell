/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_redir_helper.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 23:33:35 by gsimonia          #+#    #+#             */
/*   Updated: 2025/01/15 23:34:43 by gsimonia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_errno;

int	ft_check_access_permissions(const char *filepath, int check_write)
{
	if (check_write)
	{
		if (access(filepath, W_OK) == -1)
		{
			ft_report_error(filepath, ": Permission denied", 1);
			return (-1);
		}
	}
	else
	{
		if (access(filepath, R_OK) == -1)
		{
			ft_report_error(filepath, ": Permission denied", 1);
			return (-1);
		}
	}
	return (0);
}

int	ft_check_file_permissions(const char *filepath, int check_write)
{
	struct stat	file_stat;

	if (stat(filepath, &file_stat) == -1)
	{
		if (check_write && errno == ENOENT)
			return (0);
		ft_report_error(filepath, ": No such file or directory", 1);
		return (-1);
	}
	if (S_ISDIR(file_stat.st_mode))
	{
		ft_report_error(filepath, ": Is a directory", 1);
		return (-1);
	}
	return (ft_check_access_permissions(filepath, check_write));
}
