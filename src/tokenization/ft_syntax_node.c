/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_syntax_node.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 16:22:26 by gsimonia          #+#    #+#             */
/*   Updated: 2025/01/15 23:28:13 by anikoyan         ###   ########.fr       */
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

void	free_nodes(t_node *node)
{
	if (!node)
		return ;
	free_nodes(node->left);
	free_nodes(node->right);
	ft_node_dtor(&node);
}

void	ft_tree_dtor(t_tree **tree)
{
	if (!tree || !*tree)
		return ;
	free_nodes((*tree)->root);
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
