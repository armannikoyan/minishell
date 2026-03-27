/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_node_creation.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 00:29:47 by lvarnach          #+#    #+#             */
/*   Updated: 2026/01/27 00:45:03 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

#include "tokenization.h"
#include "utils.h"
#include "../libs/libft/libft.h"

static char	scan_token(char *input, size_t *j)
{
	char	quote;

	quote = 0;
	while (input[*j])
	{
		set_quote_char(input[*j], &quote);
		if (quote == 0 && (input[*j] == ' ' || input[*j] == '\t'
				|| is_operator(&(input[*j])) || is_redir(&(input[*j]))))
			break ;
		(*j)++;
	}
	return (quote);
}

static char	*extract_word(char *input, size_t *cur, size_t *start)
{
	*start = *cur;
	scan_token(input, cur);
	return (ft_substr(input, *start, *cur - *start));
}

static void	update_input_state(char *input, size_t *i, size_t v[2], bool skip)
{
	if (skip)
		ft_memset(input + v[1], ' ', v[0] - v[1]);
	else
		*i = v[0];
}

size_t	get_argv_len(char *input, size_t *i)
{
	size_t	j;
	size_t	len;
	char	quote;
	bool	skip;

	j = *i;
	len = 0;
	while (input[j] && !is_operator(&(input[j])))
	{
		skip_padding(input, &j, &skip);
		if (!input[j] || is_operator(&(input[j])))
			break ;
		len++;
		quote = scan_token(input, &j);
		if (quote != 0)
		{
			print_error("minishell: parsing error near unexpected token `",
				true);
			write(2, &quote, 1);
			write(2, "'\n", 2);
			return (0);
		}
	}
	return (len);
}

char	**get_argv(char *input, size_t *i)
{
	char	**argv;
	size_t	len;
	size_t	idx;
	size_t	v[2];
	bool	skip;

	len = get_argv_len(input, i);
	if (len == 0)
		return (NULL);
	argv = (char **)malloc(sizeof(char *) * (len + 1));
	if (!argv)
		return (NULL);
	idx = 0;
	v[0] = *i;
	while (idx < len)
	{
		skip_padding(input, &v[0], &skip);
		argv[idx] = extract_word(input, &v[0], &v[1]);
		if (!argv[idx])
			return (free_split(argv), NULL);
		update_input_state(input, i, v, skip);
		idx++;
	}
	argv[len] = NULL;
	return (argv);
}
