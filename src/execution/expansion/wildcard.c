/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 22:05:00 by lvarnach          #+#    #+#             */
/*   Updated: 2026/01/28 00:43:44 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <stdbool.h>

#include "execution.h"
#include "expansion.h"
#include "../../../libs/libft/libft.h"

/* Helper to process a single token: checks for wildcard or just removes quotes */
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
}

/* ** Iterates through the string, splitting by spaces only if they are
** NOT inside quotes.
*/
static void	tokenize_and_expand(t_list **final_list, char *str)
{
	int		i;
	int		start;
	char	quote;
	char	*segment;

	i = 0;
	start = 0;
	quote = 0;
	while (str[i])
	{
		// Toggle quote state if we encounter a quote char
		if ((str[i] == '\'' || str[i] == '"') && (quote == 0 || quote == str[i]))
			quote = (quote == str[i]) ? 0 : str[i];

		// If we find a space and we are NOT inside quotes, split the token
		if (!quote && str[i] == ' ')
		{
			if (i > start) // Found a word
			{
				segment = ft_substr(str, start, i - start);
				process_segment(final_list, segment);
				free(segment);
			}
			start = i + 1;
		}
		i++;
	}
	if (i > start) // Handle the last word
	{
		segment = ft_substr(str, start, i - start);
		process_segment(final_list, segment);
		free(segment);
	}
}

char	**expand_wildcards(char **old_argv)
{
	t_list	*final_list;
	char	**result;
	int		i;

	final_list = NULL;
	i = -1;
	while (old_argv[++i])
	{
		// Pass each argument to the splitter.
		// If it's a quoted string like "a b", it won't split.
		// If it's an unquoted expansion like /bin/echo 42, it will split.
		tokenize_and_expand(&final_list, old_argv[i]);
	}
	result = list_to_argv(final_list);
	ft_lstclear(&final_list, free);
	return (result);
}