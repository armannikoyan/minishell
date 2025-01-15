/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_syntax_tree_parse.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 16:22:22 by gsimonia          #+#    #+#             */
/*   Updated: 2025/01/15 20:14:50 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_node	*ft_node_ctor(char **content,
		char type, unsigned short subshell_level)
{
	t_node	*new;

	new = (t_node *)malloc(sizeof(t_node));
	if (!new)
	{
		ft_free_2d_array((void ***)(&content));
		return (NULL);
	}
	new->content = content;
	new->type = type;
	new->subshell_level = subshell_level;
	new->left = NULL;
	new->right = NULL;
	return (new);
}

static unsigned short	handle_o_token(t_list *tmp)
{
	if ((t_token *)tmp->content && ((t_token *)tmp->content)->type == 'O')
	{
		if ((((t_token *)tmp->content)->content[0] == '>'
				|| ((t_token *)tmp->content)->content[0] == '<'))
			return (2);
		else
			return (1);
	}
	return (0);
}

static void	increment_len_and_move(t_list **tmp, unsigned short *len)
{
	(*len)++;
	*tmp = (*tmp)->next;
}

static unsigned short	ft_contentlen(t_list **lst)
{
	unsigned short	len;
	t_list			*tmp;

	len = 0;
	tmp = *lst;
	if ((t_token *)tmp->content && ((t_token *)tmp->content)->type == 'X')
	{
		increment_len_and_move(&tmp, &len);
		while (tmp && (t_token *)tmp->content
			&& ((t_token *)tmp->content)->type != 'O')
			increment_len_and_move(&tmp, &len);
		while (tmp && (t_token *)tmp->content
			&& (((t_token *)tmp->content)->type == 'O'
				|| ((t_token *)tmp->content)->type == 'F'))
			tmp = tmp->next;
		while (tmp && (t_token *)tmp->content
			&& ((t_token *)tmp->content)->type == 'A')
			increment_len_and_move(&tmp, &len);
	}
	else if ((t_token *)tmp->content && ((t_token *)tmp->content)->type == 'O')
		return (handle_o_token(tmp));
	return (len);
}

t_node	*parse_node(t_list **lst)
{
	char			**content;
	char			type_of_node;
	t_list			*tmp;

	if (!lst || !*lst)
		exit(EXIT_FAILURE);
	tmp = *lst;
	content = (char **)malloc(sizeof(char *) * (ft_contentlen(&tmp) + 1));
	if (!content)
		exit(EXIT_FAILURE);
	if ((t_token *)tmp->content && ((t_token *)tmp->content)->type == 'X')
		parse_x_node(lst, &content, &type_of_node);
	else if ((t_token *)tmp->content && ((t_token *)tmp->content)->type == 'O')
		parse_o_node(lst, &content, &type_of_node);
	else
	{
		free(content);
		return (NULL);
	}
	return (ft_node_ctor(content, type_of_node,
			((t_token *)tmp->content)->subshell_level));
}
