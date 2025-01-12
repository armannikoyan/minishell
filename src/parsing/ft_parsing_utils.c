/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 16:23:03 by gsimonia          #+#    #+#             */
/*   Updated: 2025/01/12 22:15:24 by gsimonia         ###   ########.fr       */
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
		return (ft_report_error(token->content, " : is a directory", 126));
	return (false);
}

void	append_character(char *expanded_input, char c, unsigned short *j)
{
	expanded_input[*j] = c;
	(*j)++;
}
