/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 16:23:03 by gsimonia          #+#    #+#             */
/*   Updated: 2025/01/15 23:18:15 by gsimonia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	append_character(char *expanded_input, char c, unsigned short *j)
{
	expanded_input[*j] = c;
	(*j)++;
}

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
