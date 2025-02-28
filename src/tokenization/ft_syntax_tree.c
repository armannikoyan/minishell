/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_syntax_tree.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 12:47:00 by anikoyan          #+#    #+#             */
/*   Updated: 2025/02/28 14:15:16 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

short   ft_get_priority(const char *content)
{
	if (!content)
		return (-1);
	if (ft_strcmp(content, "|") == 0 || ft_strcmp(content, "||") == 0)
		return (5);
	else if (ft_strcmp(content, "&&") == 0)
		return (4);
	else if (ft_strcmp(content, "<") == 0 || ft_strcmp(content, "<<") == 0)
		return (2);
	else if (ft_strcmp(content, ">") == 0 || ft_strcmp(content, ">>") == 0)
		return (1);
	return (3);
}

void    add_to_tree(t_tree *tree, t_node *node)
{
	t_node	*current;
	short	node_priority;
	t_node	*temp;

	if (!tree || !node)
		return ;
	if (!tree->root)
	{
		tree->root = node;
		return ;
	}
	node_priority = ft_get_priority(node->content[0]);
	if (node_priority > ft_get_priority(tree->root->content[0]))
	{
		node->left = tree->root;
		tree->root = node;
		return ;
	}
	current = tree->root;
	while (current)
	{
		if (!current->right || 
				(current->right && node_priority > ft_get_priority(current->right->content[0])))
		{
			temp = current->right;
			current->right = node;
			node->left = temp;
			return;
		}
		current = current->right;
	}
}

void	ft_printf_tree(t_node *node)
{
	if (!node)
		return ;
	if (node->left)
		ft_printf("going left\n");
	ft_printf_tree(node->left);
	if (node->left)
		ft_printf("returning from left\n");
	ft_printf("---------------------------\n");
	for (int i = 0; node->content[i]; i++)
		ft_printf("%s\n", node->content[i]);
	ft_printf("---------------------------\n");
	if (node->right)
		ft_printf("going right\n");
	ft_printf_tree(node->right);
	if (node->right)
		ft_printf("returning from right\n");
}

t_tree	*ft_tree_build(t_list **lst)
{
	t_tree	*tree;
	t_list	*tmp;
	t_node	*new_node;

	tree = ft_tree_ctor();
	if (!tree)
		exit(EXIT_FAILURE);
	tmp = *lst;
	while (tmp)
	{
		new_node = parse_node(&tmp);
		if (new_node)
			add_to_tree(tree, new_node);
		tmp = tmp->next;
	}
	ft_printf("----------root-----------\n");
	for (int i = 0; tree->root->content[i]; i++)
		ft_printf("%s\n", tree->root->content[i]);
	ft_printf("---------------------------\n\n");
	ft_printf_tree(tree->root);
	return (tree);
}
