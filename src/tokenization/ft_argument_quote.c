/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_argument_quote.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 16:22:52 by gsimonia          #+#    #+#             */
/*   Updated: 2025/01/14 01:58:46 by gsimonia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	increment_counters(unsigned short *len, unsigned short *i)
{
	(*len)++;
	(*i)++;
}

static unsigned short	get_content_length_without_quotes(const char *content)
{
	unsigned short	len;
	unsigned short	i;
	char			quote;

	len = 0;
	i = 0;
	while (content[i])
	{
		if (content[i] == '\'' || content[i] == '\"')
		{
			quote = content[i++];
			while (content[i] && content[i] != quote)
				increment_counters(&len, &i);
			if (content[i])
				i++;
		}
		else
			increment_counters(&len, &i);
	}
	return (len);
}

static void	copy_content_without_quotes(char *new_content, const char *content)
{
	unsigned short	i;
	unsigned short	j;
	char			quote;

	i = 0;
	j = 0;
	while (content[i])
	{
		if (content[i] == '\'' || content[i] == '\"')
		{
			quote = content[i++];
			while (content[i] && content[i] != quote)
				new_content[j++] = content[i++];
			if (content[i])
				i++;
		}
		else
			new_content[j++] = content[i++];
	}
	new_content[j] = '\0';
}

static int	process_content_string(char **content_ptr)
{
	unsigned short	len;
	char			*new_content;

	len = get_content_length_without_quotes(*content_ptr);
	new_content = (char *)malloc(sizeof(char) * (len + 1));
	if (!new_content)
		return (0);
	copy_content_without_quotes(new_content, *content_ptr);
	free(*content_ptr);
	*content_ptr = new_content;
	return (1);
}

void	ft_quote_removal(t_node *node)
{
	unsigned short	k;

	if (!node || !node->content)
		return ;
	k = 0;
	while (node->content[k])
	{
		if (!process_content_string(&node->content[k]))
			return ;
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
