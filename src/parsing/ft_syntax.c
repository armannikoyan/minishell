/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_syntax.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 19:38:41 by anikoyan          #+#    #+#             */
/*   Updated: 2024/12/31 17:44:20 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	has_single_token_error(t_list **lst)
{
	t_token	*token;

	if (!*lst || !(*lst)->content)
		return (true);
	token = (t_token *)(*lst)->content;
	if (*lst && token && token->type != 'X'
		&& (token->content[0] != '<'
			|| token->content[0] != '>')
		&& (*lst)->next == NULL)
		return (ft_report_error("parse error near: ", token->content, 1));
	return (false);
}

bool	has_error_type_e(t_token *token)
{
	return (false); // TODO: fix this
	if (token->type == 'E')
		return (ft_report_error("command not found: ", token->content, 127));
	return (false);
}

bool	has_consecutive_operators(t_token *token, t_list *next)
{
	if (token->type == 'O' && next
		&& next->content && ((t_token *)next->content)->type == 'O'
		&& (((t_token *)next->content)->content[0] != '<'
			&& ((t_token *)next->content)
			->content[0] != '>'))
		return (ft_report_error("parse error near: ",
				((t_token *)next->content)->content, 1));
	return (false);
}

bool	has_invalid_operator_file(t_token *token, t_list *next)
{
	if (token->type == 'O' && ft_strlen(token->content) == 1
		&& (token->content[0] == '<' || token->content[0] == '>')
		&& next && ((t_token *)next->content)->type != 'F')
		return (ft_report_error("no such file or directory: ",
				((t_token *)next->content)->content, 1));
	return (false);
}

bool	has_operator_followed_by_operator(t_token *token, t_list *next)
{
	if (token->type == 'O' && (token->content[0] == '<'
			|| token->content[0] == '>')
		&& next && ((t_token *)next->content)->type == 'O'
		&& (((t_token *)next->content)->content[0] == '<'
			|| ((t_token *)next->content)->content[0] == '>'))
		return (ft_report_error("parse error near: ",
				((t_token *)next->content)->content, 1));
	return (false);
}

bool	has_unmatched_quotes(t_token *token)
{
	if (token->type == 'A' && (token->content[0] == '\''
			|| token->content[0] == '\"')
		&& (token->content[ft_strlen(token->content) - 1] != token->content[0]
			|| ft_strlen(token->content) == 1))
		return (ft_report_error("parse error near: ", token->content, 1));
	return (false);
}

bool	has_matching_parentheses(t_list *lst)
{
	short	parentheses_count;
	t_list	*tmp;
	t_token	*token;

	parentheses_count = 0;
	tmp = lst;
	while (tmp)
	{
		token = (t_token *)tmp->content;
		if (token->content[0] == '(')
			parentheses_count++;
		else if (token->content[0] == ')')
		{
			parentheses_count--;
			if (parentheses_count < 0)
				return (ft_report_error("parse error near: ",
						token->content, 1));
		}
		tmp = tmp->next;
	}
	if (parentheses_count != 0)
		return (ft_report_error("parse error near: ",
				"(", 1));
	return (false);
}

bool	has_matching_single_quotes(t_list *lst)
{
	bool			single_quotes;
	unsigned short	i;
	t_list			*tmp;
	t_token			*token;

	single_quotes = false;
	tmp = lst;
	while (tmp)
	{
		token = (t_token *)tmp->content;
		i = 0;
		while (token->content[i])
		{
			if (token->content[i] == '\''
				&& (i == 0 || token->content[i - 1] != '\\'))
				single_quotes = !single_quotes;
			i++;
		}
		tmp = tmp->next;
	}
	if (single_quotes)
		return (ft_report_error("parse error near: ",
				"\'", 1));
	return (false);
}

bool	has_matching_double_quotes(t_list *lst)
{
	bool			double_quotes;
	unsigned short	i;
	t_list			*tmp;
	t_token			*token;

	double_quotes = false;
	tmp = lst;
	while (tmp)
	{
		token = (t_token *)tmp->content;
		i = 0;
		while (token->content[i])
		{
			if (token->content[i] == '\"'
				&& (i == 0 || token->content[i - 1] != '\\'))
				double_quotes = !double_quotes;
			i++;
		}
		tmp = tmp->next;
	}
	if (double_quotes)
		return (ft_report_error("parse error near: ",
				"\"", 1));
	return (false);
}

bool	ft_has_syntax_error(t_list **lst)
{
	t_list	*tmp;
	t_token	*token;

	if (has_single_token_error(lst)
		|| has_matching_single_quotes(*lst)
		|| has_matching_double_quotes(*lst)
		|| has_matching_parentheses(*lst))
		return (true);
	tmp = *lst;
	while (tmp)
	{
		token = (t_token *)tmp->content;
		if (has_error_type_e(token)
			|| has_consecutive_operators(token, tmp->next)
			|| has_invalid_operator_file(token, tmp->next)
			|| has_operator_followed_by_operator(token, tmp->next)
			|| has_unmatched_quotes(token)
			|| (token->type == 'O' && !tmp->next
				&& ft_report_error("parse error near: ", token->content, 1)))
			return (true);
		tmp = tmp->next;
	}
	return (false);
}
