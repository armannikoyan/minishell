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

static void	add_to_list(t_list **final_list, char **old_argv, int i)
{
	char	*unquoted_str;

	unquoted_str = remove_quotes(old_argv[i]);
	ft_lstadd_back(final_list, ft_lstnew(unquoted_str));
}

char	**expand_wildcards(char **old_argv)
{
	t_list	*final_list;
	t_list	*matches;
	char	**result;
	int		i;

	final_list = NULL;
	i = -1;
	while (old_argv[++i])
	{
		if (ft_strchr(old_argv[i], '*'))
		{
			matches = get_matches(old_argv[i], NULL);
			if (matches)
				ft_lstadd_back(&final_list, matches);
			else
				add_to_list(&final_list, old_argv, i);
		}
		else
			add_to_list(&final_list, old_argv, i);
	}
	result = list_to_argv(final_list);
	ft_lstclear(&final_list, free);
	return (result);
}
