/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:07:07 by gsimonia          #+#    #+#             */
/*   Updated: 2025/01/11 11:44:05 by gsimonia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_pwd(int argc, char **argv)
{
	char	cwd[PATH_MAX];

	(void)argc;
	(void)argv;
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (ft_report_error("pwd: cannot access current directory: ",
				"No such file or directory", 1));
	write_string(cwd, "\n", NULL);
	return (EXIT_SUCCESS);
}

// #include <errno.h>

// int	ft_pwd(int argc, char **argv)
// {
// 	char	cwd[PATH_MAX];

// 	(void)argc;
// 	(void)argv;

// 	if (getcwd(cwd, sizeof(cwd)) == NULL)
// 	{
// 		if (errno == EACCES)
// 			return (ft_report_error("pwd", 
// 				"failed to get current directory: Permission denied", 2));
// 		else if (errno == ENOENT)
// 			return (ft_report_error("pwd", 
// 				"failed to get current directory: No such file or directory", 1));
// 		else
// 			return (ft_report_error("pwd", strerror(errno), 1));
// 	}
// 	write_string(cwd, "\n", NULL);
// 	return (EXIT_SUCCESS);
// }