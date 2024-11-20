/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_syntax.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 19:38:41 by anikoyan          #+#    #+#             */
/*   Updated: 2024/11/20 18:30:44 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	ft_has_syntax_error(t_list **lst)
{
	t_list	*tmp;
	t_token	*prev;
	t_token	*token;
	char	*err_msg;

	tmp = *lst;
	token = NULL;
	if (*lst && (t_token *)(*lst)->content
		&& ((t_token *)(*lst)->content)->type != 'X'
		&& ((t_token *)(*lst)->content)->type != 'O'
		&& (((t_token *)(*lst)->content)->content)[0] != '<'
		&& (((t_token *)(*lst)->content)->content)[0] != '>')
	{
		err_msg = ft_strjoin("parse error near: ",
				((t_token *)(*lst)->content)->content);
		ft_error(&err_msg, 1);
		return (true);
	}
	while (tmp)
	{
		prev = token;
		token = (t_token *)tmp->content;
		if (token->type == 'E')
		{
			err_msg = ft_strjoin("command not found: ", token->content);
			ft_error(&err_msg, 127);
			return (true);
		}
		else if (token->type == 'O'
			&& ((t_token *)tmp->next->content)->type == 'O'
			&& ((((t_token *)tmp->next->content)->content)[0] != '<'
			&& (((t_token *)tmp->next->content)->content)[0] != '>'))
		{
			err_msg = ft_strjoin("parse error near: ", token->content);
			ft_error(&err_msg, 1);
			return (true);
		}
		else if (token->type == 'O' && ft_strlen(token->content) == 1
			&& token->content[0] == '<' && prev && prev->type != 'A'
			&& prev->type != 'X')
		{
			err_msg = ft_strjoin("<command or content>: command not found ",
					token->content);
			ft_error(&err_msg, 127);
			return (true);
		}
		else if (token->type == 'O' && ft_strlen(token->content) == 1
			&& (token->content[0] == '<' || token->content[0] == '>')
			&& tmp->next && ((t_token *)tmp->next->content)->type != 'F')
		{
			err_msg = ft_strjoin("no such file or directory: ",
					((t_token *)tmp->next->content)->content);
			ft_error(&err_msg, 1);
			return (true);
		}
		else if (token->type == '0' && ((token->content)[0] == '<'
			|| (token->content)[0] == '>') && tmp->next
			&& ((t_token *)tmp->next->content)->type == 'O'
			&& (((t_token *)tmp->next->content)->content[0] == '<'
			|| ((t_token *)tmp->next->content)->content[0] == '>'))
		{
			err_msg = ft_strjoin("parse error near: ",
					((t_token *)tmp->next->content)->content);
			ft_error(&err_msg, 1);
			return (true);
		}
		tmp = tmp->next;
	}
	return (false);
}
