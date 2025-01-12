/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_syntax.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 19:38:41 by anikoyan          #+#    #+#             */
/*   Updated: 2025/01/12 07:47:48 by gsimonia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
		if (token->content[0] == ')' || token->content[1] == ')')
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

bool	ft_has_syntax_error(t_list **lst)
{
	t_list	*tmp;
	t_token	*token;

	if (has_single_token_error(lst) || has_matching_parentheses(*lst))
		return (true);
	tmp = *lst;
	while (tmp)
	{
		token = (t_token *)tmp->content;
		if (has_consecutive_operators(token, tmp->next)
			|| has_invalid_operator_file(token, tmp->next)
			|| has_operator_followed_by_operator(token, tmp->next)
			|| (token->type == 'O' && !tmp->next
				&& ft_report_error("parse error near: ", token->content, 1)))
			return (true);
		tmp = tmp->next;
	}
	return (false);
}
