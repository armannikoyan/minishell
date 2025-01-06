/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:07:07 by gsimonia          #+#    #+#             */
/*   Updated: 2025/01/06 18:34:18 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_errno;

int	ft_pwd(int argc, char **argv)
{
	char	cwd[PATH_MAX];

	(void)argc;
	(void)argv;
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (write_error("pwd: ", strerror(g_errno), NULL));
	write_string(cwd, "\n", NULL);
	return (EXIT_SUCCESS);
}
