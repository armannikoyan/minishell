/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 22:05:00 by lvarnach          #+#    #+#             */
/*   Updated: 2026/02/03 01:08:36 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <stdbool.h>

#include "execution.h"
#include "expansion.h"
#include "tokenization.h"
#include "../../../libs/libft/libft.h"

static void	process_segment(t_list **final_list, char *segment)
{
	t_list	*matches;
	char	*unquoted_str;

	if (ft_strchr(segment, '*'))
	{
		matches = get_matches(segment, NULL);
		if (matches)
			ft_lstadd_back(final_list, matches);
		else
		{
			unquoted_str = remove_quotes(segment);
			ft_lstadd_back(final_list, ft_lstnew(unquoted_str));
		}
	}
	else
	{
		unquoted_str = remove_quotes(segment);
		ft_lstadd_back(final_list, ft_lstnew(unquoted_str));
	}
	free(segment);
}

static void	tokenize_and_expand(t_list **final_list, char *str)
{
	int		i;
	int		start;
	char	quote;

	i = -1;
	start = 0;
	quote = 0;
	while (str[++i])
	{
		if ((str[i] == '\'' || str[i] == '"')
			&& (quote == 0 || quote == str[i]))
			set_quote_char(str[i], &quote);
		if (!quote && str[i] == ' ')
		{
			if (i > start)
				process_segment(final_list, ft_substr(str, start, i - start));
			start = i + 1;
		}
	}
	if (i > start)
		process_segment(final_list, ft_substr(str, start, i - start));
}

char	**expand_wildcards(char **old_argv)
{
	t_list	*final_list;
	char	**result;
	int		i;

	final_list = NULL;
	i = -1;
	while (old_argv[++i])
		tokenize_and_expand(&final_list, old_argv[i]);
	result = list_to_argv(final_list);
	ft_lstclear(&final_list, free);
	return (result);
}
