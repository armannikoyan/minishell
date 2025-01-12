/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_syntax_node.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 16:22:26 by gsimonia          #+#    #+#             */
/*   Updated: 2025/01/13 00:21:57 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_node_dtor(t_node **node)
{
	if (!node || !*node)
		return ;
	ft_free_2d_array((void ***)(&(*node)->content));
	free(*node);
	*node = NULL;
}

t_tree	*ft_tree_ctor(void)
{
	t_tree	*new;

	new = (t_tree *)malloc(sizeof(t_tree));
	if (!new)
		return (NULL);
	new->root = NULL;
	return (new);
}

void	ft_tree_dtor(t_tree **tree)
{
	t_node	*current;

	if (!tree || !*tree)
		return ;
	current = (*tree)->root;
	while (current)
	{
		if (current->left)
			ft_node_dtor(&current->left);
		if (current->right)
			ft_node_dtor(&current->right);
		ft_node_dtor(&current);
	}
	free(*tree);
	*tree = NULL;
}

void	parse_x_node(t_list **lst, char ***content, char *type_of_node)
{
	t_list			*tmp;
	unsigned short	i;

	*type_of_node = 'X';
	tmp = *lst;
	i = 0;
	while (tmp && (t_token *)tmp->content
		&& ((t_token *)tmp->content)->type != 'O')
	{
		(*content)[i++] = ((t_token *)tmp->content)->content;
		tmp = tmp->next;
	}
	while (tmp && (t_token *)tmp->content
		&& (((t_token *)tmp->content)->type == 'O'
			|| ((t_token *)tmp->content)->type == 'F'))
		tmp = tmp->next;
	while (tmp && (t_token *)tmp->content
		&& ((t_token *)tmp->content)->type == 'A')
	{
		(*content)[i++] = ((t_token *)tmp->content)->content;
		tmp = tmp->next;
	}
	(*content)[i] = NULL;
}

void	parse_o_node(t_list **lst, char ***content, char *type_of_node)
{
	t_list			*tmp;
	unsigned short	i;

	*type_of_node = 'O';
	tmp = *lst;
	i = 0;
	(*content)[i++] = ((t_token *)tmp->content)->content;
	tmp = tmp->next;
	if (tmp && (t_token *)tmp->content
		&& ((t_token *)tmp->content)->type == 'F')
	{
		(*content)[i++] = ((t_token *)tmp->content)->content;
		tmp = tmp->next;
	}
	(*content)[i] = NULL;
}
