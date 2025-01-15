/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_argument_manipulation.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:34:14 by anikoyan          #+#    #+#             */
/*   Updated: 2025/01/15 17:34:56 by gsimonia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_insert_new_node(t_list **lst, t_list *new_node)
{
	t_list	*current;
	t_list	*previous;

	if (!lst || !new_node)
		return ;
	if (!*lst || ft_strcmp(((t_token *)new_node->content)->content,
			((t_token *)(*lst)->content)->content) < 0)
	{
		new_node->next = *lst;
		*lst = new_node;
		return ;
	}
	current = *lst;
	previous = NULL;
	while (current && ft_strcmp(((t_token *)new_node->content)->content,
			((t_token *)current->content)->content) >= 0)
	{
		previous = current;
		current = current->next;
	}
	new_node->next = current;
	previous->next = new_node;
}

void	ft_create_and_insert_new_node(t_list **lst,
		const char *path, const char *entry_name)
{
	t_token	*new_token;
	t_list	*new_node;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		exit(EXIT_FAILURE);
	new_token->content = ft_strjoin(path, entry_name);
	if (!new_token->content)
		exit(EXIT_FAILURE);
	new_node = ft_lstnew(new_token);
	if (!new_node)
		exit(EXIT_FAILURE);
	ft_insert_new_node(lst, new_node);
}

bool	ft_close_directory(DIR *dir)
{
	if (closedir(dir) == -1)
		ft_report_error("closedir failed: ", NULL, 1);
	return (true);
}

unsigned short	ft_path_len(char *str)
{
	unsigned short	i;

	i = 0;
	while (str[i] && str[i] != '*')
		i++;
	while (str[i] && &str[i] != str && str[i] != '/')
		i--;
	if (str[i] == '/')
		i++;
	if (&str[i] == str)
		return (0);
	return (&str[i] - str);
}

bool	ft_process_path_patterns(t_list ***lst_ref)
{
	t_list	*current ;
	t_list	*prev;
	t_list	*next;
	t_token	*token;

	current = **lst_ref;
	prev = NULL;
	while (current)
	{
		next = current->next;
		token = (t_token *)current->content;
		if (ft_is_pattern_match(token))
		{
			if (!ft_process_pattern(lst_ref, current, prev, token))
				return (false);
		}
		else
			prev = current;
		current = next;
	}
	return (true);
}
