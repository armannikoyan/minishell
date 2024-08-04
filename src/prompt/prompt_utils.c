/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasargsy <dasargsy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 18:44:53 by dasargsy          #+#    #+#             */
/*   Updated: 2024/08/04 17:25:41 by dasargsy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_for_quotes(char *line, int limiter)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	while (line[i])
	{
		if (line[i] == limiter)
			flag++;
		i++;
	}
	if (flag % 2 == 0)
		return (1);
	if (flag % 2 != 0)
		return (2);
	return (0);
}

static	int	is_spc_bef(char *line)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	while (line[i] && line[i] != '(')
	{
		if (ft_isspace(line[i]))
			flag++;
		i++;
	}
	if (flag == i)
		return (1);
	return (0);
}

int	check_for_parenthesis(char *line)
{
	int	i;
	int	lft;
	int	rght;

	i = 0;
	lft = 0;
	rght = 0;
	while (line[i])
	{
		if (line[i] == '(')
			lft++;
		if (line[i] == ')')
			rght++;
		i++;
	}
	if (lft > 0 && rght == 0 && is_spc_bef(line))
		return (2);
	else if (lft == rght)
		return (1);
	else if (lft != rght)
		return (0);
	return (1);
}

