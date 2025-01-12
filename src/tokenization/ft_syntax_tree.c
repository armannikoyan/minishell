/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_syntax_tree.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 12:47:00 by anikoyan          #+#    #+#             */
/*   Updated: 2025/01/13 02:06:08 by anikoyan         ###   ########.fr       */
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
	if ((ft_strcmp(tree->root->content[0], "<") == 0 || ft_strcmp(tree->root->content[0], ">") == 0)
		&& ft_strcmp(tree->root->content[0], node->content[0]) == 0)
	{
		node->left = tree->root->left;
		node->right = tree->root->right;
		tree->root->left = node;
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
