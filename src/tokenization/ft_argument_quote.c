/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_argument_quote.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 16:22:52 by gsimonia          #+#    #+#             */
/*   Updated: 2025/01/13 07:02:31 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_quote_removal(t_node *node)
{
	char			quote;
	char			*new_content;
	unsigned short	len;
	unsigned short	i;
	unsigned short	j;
	unsigned short	k;

	if (!node || !node->content)
		return ;
	k = 0;
	while (node->content[k])
	{
		len = 0;
		i = 0;
		while (node->content[k][i])
		{
			if (node->content[k][i] == '\'' || node->content[k][i] == '\"')
			{
				quote = node->content[k][i++];
				while (node->content[k][i] && node->content[k][i] != quote)
				{
					len++;
					i++;
				}
				if (node->content[k][i])
					i++;
			}
			else
			{
				len++;
				i++;
			}
		}
		new_content = (char *)malloc(sizeof(char) * (len + 1));
		if (!new_content)
			return ;
		i = 0;
		j = 0;
		while (node->content[k][i])
		{
			if (node->content[k][i] == '\'' || node->content[k][i] == '\"')
			{
				quote = node->content[k][i++];
				while (node->content[k][i] && node->content[k][i] != quote)
					new_content[j++] = node->content[k][i++];
				if (node->content[k][i])
					i++;
			}
			else
				new_content[j++] = node->content[k][i++];
		}
		new_content[j] = '\0';
		free(node->content[k]);
		node->content[k] = new_content;
		k++;
	}
}

bool	ft_is_pattern_match(t_token *token)
{
	char			quote;
	unsigned short	i;

	i = 0;
	while (token->content[i])
	{
		while (token->content[i] && token->content[i] != '\''
			&& token->content[i] != '\"')
		{
			if (token->content[i] == '*')
				return (true);
			i++;
		}
		if (token->content[i])
		{
			quote = token->content[i];
			i++;
		}
		while (token->content[i] && token->content[i] != quote)
			i++;
		if (token->content[i])
			i++;
	}
	return (false);
}
