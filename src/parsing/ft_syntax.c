/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_syntax.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 19:38:41 by anikoyan          #+#    #+#             */
/*   Updated: 2025/01/15 19:07:31 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	has_consecutive_operators(t_token *token, t_list *next)
{
	if (token->type == 'O' && next
		&& next->content && ((t_token *)next->content)->type == 'O'
		&& ((t_token *)next->content)->content[0] != '<'
		&& ((t_token *)next->content)->content[0] != '>')
		return (ft_report_error("parse error near: ",
				((t_token *)next->content)->content, 258));
	return (false);
}

static bool	handle_parentheses_error(int parentheses_count)
{
	if (parentheses_count > 0)
		return (ft_report_error("parse error near: ", "(", 258));
	else if (parentheses_count < 0)
		return (ft_report_error("parse error near: ", ")", 258));
	return (false);
}

static void	update_quote_state(char current_char, char *quote, bool *in_quote)
{
	*quote = current_char;
	*in_quote = !(*in_quote);
	if (*in_quote == false)
		*quote = 0;
}

bool	has_unmatching_parentheses(t_list *lst)
{
	t_token			*token;
	unsigned short	i;
	int				parentheses_count;
	char			quote;
	bool			in_quote;

	parentheses_count = 0;
	in_quote = false;
	i = 0;
	token = lst->content;
	quote = 0;
	while (token->content[i])
	{
		if ((token->content[i] == '\'' || token->content[i] == '\"')
			&& ((token->content[i] == quote) || (quote == 0)))
			update_quote_state(token->content[i], &quote, &in_quote);
		else if (token->content[i] == '(' && !in_quote)
			parentheses_count++;
		else if (token->content[i] == ')' && !in_quote)
			parentheses_count--;
		i++;
	}
	return (handle_parentheses_error(parentheses_count));
}

bool	ft_only_operator(t_token *token, t_list *tmp, t_list *prev)
{
	if (token->type == 'O')
	{
		if ((token->content[0] == '<'
				|| token->content[0] == '>') && !tmp->next)
			return (ft_report_error("parse error near: ", "newline", 258));
		if (token->content[0] != '<'
			&& token->content[0] != '>' && (!tmp->next || !prev))
			return (ft_report_error("parse error near: ", token->content, 258));
	}
	return (false);
}

bool	ft_has_syntax_error(t_list **lst)
{
	t_list	*tmp;
	t_token	*token;
	t_list	*prev;

	tmp = *lst;
	prev = NULL;
	while (tmp)
	{
		token = (t_token *)tmp->content;
		if (ft_only_operator(token, tmp, prev)
			|| has_consecutive_operators(token, tmp->next)
			|| has_unmatching_parentheses(tmp))
			return (true);
		prev = tmp;
		tmp = tmp->next;
	}
	return (false);
}
