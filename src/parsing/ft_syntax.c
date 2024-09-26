/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 19:38:41 by anikoyan          #+#    #+#             */
/*   Updated: 2024/09/23 17:35:54 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	ft_has_syntax_error_helper(short quote_count, short dquote_count
		, short bracket_count)
{
	if (quote_count % 2)
	{
		ft_error("parse error near `\'`", EXIT_FAILURE);
		return (1);
	}
	else if (dquote_count % 2)
	{
		ft_error("parse error near `\"`", EXIT_FAILURE);
		return (1);
	}
	else if (bracket_count > 0)
	{
		ft_error("parse error near `(`", EXIT_FAILURE);
		return (1);
	}
	else if (bracket_count < 0)
	{
		ft_error("parse error near `)`", EXIT_FAILURE);
		return (1);
	}
	return (0);
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
