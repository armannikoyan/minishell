/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_node_creation_helper.c                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 00:09:54 by lvarnach          #+#    #+#             */
/*   Updated: 2026/01/27 00:45:32 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

#include "error_codes.h"
#include "../../includes/tokenization.h"
#include "../../includes/utils.h"
#include "../../libs/libft/libft.h"

static void	get_subshell_nesting_info(char *input, size_t *i, int *nested_level)
{
	char	quote_char;

	quote_char = 0;
	while (input[*i])
	{
		set_quote_char(input[*i], &quote_char);
		if (quote_char == 0)
		{
			if (input[*i] == '(')
				++(*nested_level);
			else if (input[*i] == ')')
				--(*nested_level);
			if (*nested_level == 0)
				break ;
		}
		++(*i);
	}
}

char	*extract_subshell_content(char *input, size_t *i, int *errnum)
{
	size_t	start;
	char	*tmp;
	int		nested_level;

	start = ++(*i);
	nested_level = 1;
	get_subshell_nesting_info(input, i, &nested_level);
	if (nested_level != 0)
	{
		print_error("minishell: parsing error near unexpected token `('\n",
			true);
		*errnum = SYNTAX_ERROR;
		return (NULL);
	}
	tmp = ft_substr(input, start, *i - start);
	if (input[*i] == ')')
		++(*i);
	return (tmp);
}

char	*substr_next(char *input, size_t *i)
{
	size_t		start;
	char		quote_char;

	while (input[*i] == ' ' || input[*i] == '\t')
		++(*i);
	start = *i;
	quote_char = 0;
	while (input[*i])
	{
		set_quote_char(input[*i], &quote_char);
		if (quote_char == 0 && (input[*i] == ' ' || input[*i] == '\t'
				|| is_operator(&(input[*i])) || is_redir(&(input[*i]))))
			break ;
		++(*i);
	}
	return (ft_substr(input, start, *i - start));
}

void	skip_redirection(char *input, size_t *j)
{
	char	quote_char;

	if (is_redir(&(input[*j + 1])) && input[*j] == input[*j + 1])
		*j += 2;
	else
		(*j)++;
	while (input[*j] == ' ' || input[*j] == '\t')
		(*j)++;
	quote_char = 0;
	while (input[*j])
	{
		set_quote_char(input[*j], &quote_char);
		if (quote_char == 0 && (input[*j] == ' ' || input[*j] == '\t'
				|| is_operator(&(input[*j])) || is_redir(&(input[*j]))))
			break ;
		(*j)++;
	}
}

void	skip_padding(char *input, size_t *cur, bool *skipped)
{
	*skipped = false;
	while (input[*cur])
	{
		while (input[*cur] == ' ' || input[*cur] == '\t')
			(*cur)++;
		if (input[*cur] && is_redir(&(input[*cur])))
		{
			skip_redirection(input, cur);
			*skipped = true;
		}
		else
			break ;
	}
}
