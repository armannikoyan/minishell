/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tok_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 16:22:13 by gsimonia          #+#    #+#             */
/*   Updated: 2025/01/14 01:03:00 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_assign_argument_type(t_list **tmp)
{
	while (*tmp && (*tmp)->next && (t_token *)(*tmp)->next->content
		&& ((t_token *)(*tmp)->next->content)->content
		&& !ft_isoperator(((t_token *)(*tmp)->next->content)->content))
	{
		((t_token *)(*tmp)->next->content)->type = 'A';
		*tmp = (*tmp)->next;
	}
}

void	ft_assign_command_type(t_token *token, t_list **tmp)
{
	token->type = 'X';
	ft_assign_argument_type(tmp);
}

void	ft_assign_operator_type(t_list **tmp, t_token *token)
{
	t_token	*next_token;

	token->type = 'O';
	if ((token->content[0] == '>' || token->content[0] == '<') && (*tmp)->next)
	{
		*tmp = (*tmp)->next;
		next_token = (t_token *)(*tmp)->content;
		if (next_token)
		{
			next_token->type = 'F';
			while ((*tmp)->next && (t_token *)(*tmp)->next->content
					&& !ft_isoperator(((t_token *)(*tmp)
							->next->content)->content))
			{
				*tmp = (*tmp)->next;
				((t_token *)(*tmp)->content)->type = 'A';
			}
		}
	}
}

void	ft_assign_token_type(t_list ***lst)
{
	t_list		*tmp;
	t_token		*token;

	tmp = **lst;
	while (tmp)
	{
		token = (t_token *)tmp->content;
		if (ft_isbuiltin(token))
			ft_handle_argument(&tmp, token);
		else if (ft_identify_command(&token, ft_split(getenv("PATH"), ':')))
			ft_assign_command_type(token, &tmp);
		else if (ft_isoperator(token->content))
			ft_assign_operator_type(&tmp, token);
		else if (ft_strcmp(token->content, "(") == 0
			|| ft_strcmp(token->content, ")") == 0)
			token->type = 'S';
		else
			ft_handle_argument(&tmp, token);
		if (tmp)
			tmp = tmp->next;
	}
}

void	ft_fill_content(t_token *token,
	char *input, unsigned short *index, unsigned short len)
{
	unsigned short	i;

	token->content = (char *)malloc(sizeof(char) * (len + 1));
	if (!token->content)
		exit(EXIT_FAILURE);
	i = 0;
	while (input[*index] && input[*index] != ' ')
	{
		if (input[*index] == '\"' || input[*index] == '\'')
		{
			token->content[i++] = input[(*index)++];
			while (input[*index] && input[*index] != '\"'
				&& input[*index] != '\'')
				token->content[i++] = input[(*index)++];
		}
		token->content[i++] = input[(*index)++];
	}
	token->content[i] = '\0';
	while (input[*index] && input[*index] == ' ')
		(*index)++;
}
