/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_syntax.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 19:38:41 by anikoyan          #+#    #+#             */
/*   Updated: 2024/12/07 16:26:57 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	report_error(const char *prefix, const char *content, int error_code)
{
	char	*err_msg;

	err_msg = ft_strjoin(prefix, content);
	ft_error(&err_msg, error_code);
	return (true);
}

bool	has_single_token_error(t_list **lst)
{
	t_token	*token;

	token = (t_token *)(*lst)->content;
	if (*lst && token && token->type != 'X'
		&& (token->content[0] != '<'
			|| token->content[0] != '>')
		&& (*lst)->next == NULL)
		return (report_error("parse error near: ", token->content, 1));
	return (false);
}

bool	has_error_type_e(t_token *token)
{
	if (token->type == 'E')
		return (report_error("command not found: ", token->content, 127));
	return (false);
}

bool	has_consecutive_operators(t_token *token, t_list *next)
{
	if (token->type == 'O' && next
		&& next->content && ((t_token *)next->content)->type == 'O'
		&& (((t_token *)next->content)->content[0] != '<'
			&& ((t_token *)next->content)
			->content[0] != '>'))
		return (report_error("parse error near: ",
				((t_token *)next->content)->content, 1));
	return (false);
}

bool	has_invalid_operator_file(t_token *token, t_list *next)
{
	if (token->type == 'O' && ft_strlen(token->content) == 1
		&& (token->content[0] == '<' || token->content[0] == '>')
		&& next && ((t_token *)next->content)->type != 'F')
		return (report_error("no such file or directory: ",
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
		return (report_error("parse error near: ",
				((t_token *)next->content)->content, 1));
	return (false);
}

bool	has_unmatched_quotes(t_token *token)
{
	if (token->type == 'A' && (token->content[0] == '\''
			|| token->content[0] == '\"')
		&& (token->content[ft_strlen(token->content) - 1] != token->content[0]
			|| ft_strlen(token->content) == 1))
		return (report_error("parse error near: ", token->content, 1));
	return (false);
}

bool	ft_has_syntax_error(t_list **lst)
{
	t_list	*tmp;
	t_token	*token;

	if (has_single_token_error(lst))
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
				&& report_error("parse error near: ", token->content, 1)))
			return (true);
		tmp = tmp->next;
	}
	return (false);
}