/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_argument_quote.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 16:22:52 by gsimonia          #+#    #+#             */
/*   Updated: 2025/01/11 05:35:13 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_quote_removal(t_token *token)
{
	char	quote;
	char	*new_content;
	unsigned short	len;
	unsigned short	i;
	unsigned short	j;

	if (!token || !token->content)
		return ;
	len = 0;
	i = 0;
	while (token->content[i])
	{
		while (token->content[i] && token->content[i] != '\'' && token->content[i] != '\"')
		{
			len++;
			i++;
		}
		if (token->content[i])
		{
			quote = token->content[i];
			i++;
		}
		while (token->content[i] && token->content[i] != quote)
		{
			len++;
			i++;
		}
		if (token->content[i])
			i++;
	}
	if (len && len == ft_strlen(token->content))
		return ;
	new_content = (char *)malloc(sizeof(char) * (len + 1));
	i = 0;
	j = 0;
	while (token->content[i])
	{
		while (token->content[i] && token->content[i] != '\'' && token->content[i] != '\"')
			new_content[j++] = token->content[i++];
		if (token->content[i])
		{
			quote = token->content[i];
			i++;
		}
		while (token->content[i] && token->content[i] != quote)
			new_content[j++] = token->content[i++];
		if (token->content[i])
			i++;
	}
	new_content[j] = '\0';
	free(token->content);
	token->content = new_content;
}

bool	ft_is_pattern_match(t_token *token)
{
	char	quote;
	unsigned short	i;

	i = 0;
	while (token->content[i])
	{
		while (token->content[i] && token->content[i] != '\'' && token->content[i] != '\"')
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
