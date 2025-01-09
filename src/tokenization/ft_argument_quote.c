/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_argument_quote.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 16:22:52 by gsimonia          #+#    #+#             */
/*   Updated: 2025/01/09 16:22:53 by gsimonia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_starting_quote(char c)
{
	return (c == '\'' || c == '\"');
}

static char	*process_quote_remove(const char *content, char starting_quote)
{
	size_t	i;
	size_t	j;
	char	*result;

	result = malloc(ft_strlen(content) + 1);
	if (!result)
	{
		perror("Memory allocation failed");
		exit(EXIT_FAILURE);
	}
	i = 1;
	j = 0;
	while (content[i])
	{
		if (content[i] == starting_quote)
		{
			i++;
			continue ;
		}
		result[j++] = content[i++];
	}
	result[j] = '\0';
	return (result);
}

void	ft_quote_removal(t_token *token)
{
	char	starting_quote;
	char	*processed_content;

	if (!token || !token->content)
		return ;
	starting_quote = token->content[0];
	if (!is_starting_quote(starting_quote))
		return ;
	processed_content = process_quote_remove(token->content, starting_quote);
	free(token->content);
	token->content = processed_content;
}

bool	ft_is_pattern_match(t_token *token)
{
	return (token && token->content && ft_strchr(token->content, '*')
		&& token->content[0] != '\'' && token->content[0] != '\"');
}
