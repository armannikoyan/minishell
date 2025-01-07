/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:03:48 by gsimonia          #+#    #+#             */
/*   Updated: 2025/01/07 21:59:40 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// ISSUE: no need to expand variables on command side
// FIX: needed asap
static char	*expand_variable(const char *str)
{
	char	*expanded;

	if (str[0] == '$' && str[1] != '\0')
		expanded = getenv(str + 1);
	else
		expanded = (char *)str;
	if (expanded == NULL)
		expanded = "";
	return (expanded);
}

static bool	is_n_flag(const char *str)
{
	int	i;

	if (str[0] != '-' || str[1] != 'n')
		return (false);
	i = 2;
	while (str[i] == 'n')
		i++;
	return (str[i] == '\0');
}

static int	handle_flags(int argc, char **argv, bool *n_flag)
{
	int	i;

	i = 1;
	*n_flag = false;
	while (i < argc && is_n_flag(argv[i]))
	{
		*n_flag = true;
		i++;
	}
	if (i < argc && ft_strcmp(argv[i], "--") == 0)
		i++;
	return (i);
}

int	ft_echo(int argc, char **argv)
{
	int		i;
	bool	n_flag;
	char	*expanded;

	i = handle_flags(argc, argv, &n_flag);
	while (i < argc)
	{
		expanded = expand_variable(argv[i]);
		if (write(STDOUT_FILENO, expanded, ft_strlen(expanded)) == -1)
			return (write_error("ft_echo: ",
					"write error on argument ", argv[i]));
		if (i + 1 < argc && write(STDOUT_FILENO, " ", 1) == -1)
			return (write_error("ft_echo: ",
					"write error on space", NULL));
		i++;
	}
	if (!n_flag && write(STDOUT_FILENO, "\n", 1) == -1)
		return (write_error("ft_echo: ", "write error on newline", NULL));
	return (EXIT_SUCCESS);
}
