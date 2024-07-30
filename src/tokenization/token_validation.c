/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_validation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasargsy <dasargsy@student.42yerevan.am    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 02:44:27 by dasargsy          #+#    #+#             */
/*   Updated: 2024/07/31 03:13:34 by dasargsy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static	int	check_for_operations(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (ft_strncmp(line + i, "&&&", 3) == 0)
			return (0);
		if (ft_strncmp(line + i, "|||", 3) == 0)
			return (0);
		if (ft_strncmp, line + i, "&||", 3 == 0)
			return (0);
	}
	
}

static	int	check_for_quotes(char *line, char quote)
{
	int	quote_counter;
	int	i;

	i = 0;
	quote_counter = 0;
	while (line[i])
	{
		if (line[i] == quote)
			quote_counter++;
		i++;
	}
	if (quote_counter % 2 == 0)
		return (1);
	return (0);
}

int	valid_tokens(char *line)
{
	int	quotes;
	int	parenthesis;

	quotes = check_for_quotes(line, 96) 
		&& check_for_quotes(line, 34) && check_for_quotes(line, 39);
	parenthesis = check_for_parenthesis(line);
	return (quotes);
}