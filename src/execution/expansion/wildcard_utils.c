/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 00:48:20 by lvarnach          #+#    #+#             */
/*   Updated: 2026/01/28 01:09:00 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>

#include "expansion.h"
#include "tokenization.h"

static char	*get_pattern_mask(char *str, int **mask_out)
{
	char	*cl;
	int		*mk;
	int		v[2];
	char	q[2];

	cl = ft_calloc(ft_strlen(str) + 1, 1);
	mk = ft_calloc(ft_strlen(str) + 1, 4);
	if (!cl || !mk)
		return (free(cl), free(mk), NULL);
	v[0] = -1;
	v[1] = 0;
	q[0] = 0;
	while (str[++v[0]])
	{
		q[1] = q[0];
		set_quote_char(str[v[0]], &q[0]);
		if (q[1] == q[0])
		{
			mk[v[1]] = (q[0] != 0);
			cl[v[1]++] = str[v[0]];
		}
	}
	*mask_out = mk;
	return (cl);
}

static bool	match_pattern(char *pattern, int *mask, char *string)
{
	if (*pattern == '\0' && *string == '\0')
		return (true);
	if (*pattern == '*' && *mask == 0)
	{
		if (match_pattern(pattern + 1, mask + 1, string))
			return (true);
		if (*string && match_pattern(pattern, mask, string + 1))
			return (true);
		return (false);
	}
	if (*pattern == *string)
		return (match_pattern(pattern + 1, mask + 1, string + 1));
	return (false);
}

static void	sort_ascii(t_list *lst)
{
	t_list	*head;
	t_list	*node;
	char	*tmp_content;

	if (!lst)
		return ;
	head = lst;
	while (head)
	{
		node = head->next;
		while (node)
		{
			if (ft_strcmp(head->content, node->content) > 0)
			{
				tmp_content = head->content;
				head->content = node->content;
				node->content = tmp_content;
			}
			node = node->next;
		}
		head = head->next;
	}
}

t_list	*get_matches(char *raw_token, DIR *dir)
{
	struct dirent		*entry;
	t_list				*matches;
	char				*clean_pattern;
	int					*mask;

	matches = NULL;
	clean_pattern = get_pattern_mask(raw_token, &mask);
	if (!clean_pattern)
		return (NULL);
	dir = opendir(".");
	if (!dir)
		return (free(clean_pattern), free(mask), NULL);
	entry = readdir(dir);
	while (entry != NULL)
	{
		// FIX: Removed explicit exclusion of "." and ".."
		// Now we only check if the file is hidden and if the pattern allows it.
		if (entry->d_name[0] != '.' || clean_pattern[0] == '.')
		{
			if (match_pattern(clean_pattern, mask, entry->d_name))
				ft_lstadd_back(&matches, ft_lstnew(ft_strdup(entry->d_name)));
		}
		entry = readdir(dir);
	}
	(closedir(dir), free(clean_pattern), free(mask), sort_ascii(matches));
	return (matches);
}

char	**list_to_argv(t_list *lst)
{
	char		**new_argv;
	int			size;
	int			i;

	size = ft_lstsize(lst);
	new_argv = malloc(sizeof(char *) * (size + 1));
	if (!new_argv)
		return (NULL);
	i = 0;
	while (lst)
	{
		new_argv[i++] = ft_strdup(lst->content);
		lst = lst->next;
	}
	new_argv[i] = NULL;
	return (new_argv);
}
