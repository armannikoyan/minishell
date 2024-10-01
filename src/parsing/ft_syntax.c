/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_syntax.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 19:38:41 by anikoyan          #+#    #+#             */
/*   Updated: 2024/10/01 20:51:25 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	ft_has_syntax_error_helper(short quote_count, short dquote_count
		, short bracket_count)
{
	if (quote_count % 2)
	{
		ft_error("parse error near `\'`", EXIT_FAILURE);
		return (true);
	}
	else if (dquote_count % 2)
	{
		ft_error("parse error near `\"`", EXIT_FAILURE);
		return (true);
	}
	else if (bracket_count > 0)
	{
		ft_error("parse error near `(`", EXIT_FAILURE);
		return (true);
	}
	else if (bracket_count < 0)
	{
		ft_error("parse error near `)`", EXIT_FAILURE);
		return (true);
	}
	return (false);
}

bool	ft_has_syntax_error(char *input)
{
	short	quote_count;
	short	dquote_count;
	short	bracket_count;

	quote_count = 0;
	dquote_count = 0;
	bracket_count = 0;
	while (*input)
	{
		if (*input == '\'')
			quote_count++;
		else if (*input == '"')
			dquote_count++;
		else if (*input == '(')
			bracket_count++;
		else if (*input == ')')
			bracket_count--;
		if (bracket_count < 0)
			break ;
		input++;
	}
	return (ft_has_syntax_error_helper(quote_count, dquote_count
			, bracket_count));
}
