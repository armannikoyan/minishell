/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:07:07 by gsimonia          #+#    #+#             */
/*   Updated: 2024/11/20 14:56:35 by gsimonia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_pwd(int argc, char **argv)
{
	char	cwd[PATH_MAX];

	(void)argc;
	(void)argv;
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (write_error("pwd: ", strerror(errno), NULL));
	write_string(cwd, "\n", NULL);
	return (EXIT_SUCCESS);
}
