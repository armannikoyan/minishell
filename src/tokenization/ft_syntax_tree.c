/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_syntax_tree.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 12:47:00 by anikoyan          #+#    #+#             */
/*   Updated: 2025/01/13 06:55:43 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	add_operator_node(t_tree *tree, t_node *node)
{
	t_node	*current;

	current = tree->root;
	if (!current)
	{
		tree->root = node;
		return ;
	}
	if (ft_strncmp(node->content[0], "<", 1) == 0
		|| ft_strncmp(node->content[0], ">", 1) == 0)
	{
		if (ft_strcmp(current->content[0], "|") == 0)
		{
			if (!current->right)
				current->right = node;
			else
			{
				node->left = current->right;
				current->right = node;
			}
		}
		else if (ft_strcmp(current->content[0], node->content[0]) == 0)
		{
			node->left = current->left;
			node->right = current->right;
			current->left = node;
		}
		else if (ft_strncmp(node->content[0], "<", 1) == 0
			&& ft_strncmp(tree->root->content[0], ">", 1) == 0)
		{
			node->left = current->left;
			node->right = current->right;
			current->left = node;
			current->right = NULL;
		}
		else
		{
			node->left = current;
			tree->root = node;
		}
	}
	else
	{
		node->left = tree->root;
		tree->root = node;
	}
}

static void	add_non_operator_node(t_tree *tree, t_node *node)
{
	t_node	*current;

	current = tree->root;
	while (current->right)
		current = current->right;
	if (!current->left)
		current->left = node;
	else
		current->right = node;
}

static void	add_to_tree(t_tree *tree, t_node *node)
{
	if (!tree || !node)
		return ;
	if (!tree->root)
	{
		tree->root = node;
		return ;
	}
	if (node->type == 'O')
		add_operator_node(tree, node);
	else
		add_non_operator_node(tree, node);
}

t_tree	*ft_tree_build(t_list **lst)
{
	t_tree	*tree;
	t_list	*tmp;
	t_node	*new_node;

	if (!lst || !*lst)
		return (NULL);
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
	return (tree);
}
