/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:07:07 by gsimonia          #+#    #+#             */
/*   Updated: 2025/01/12 06:53:31 by gsimonia         ###   ########.fr       */
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
