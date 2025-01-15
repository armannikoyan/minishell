/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_syntax_tree_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 23:30:32 by anikoyan          #+#    #+#             */
/*   Updated: 2025/01/15 23:32:18 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_redirection_operator(t_tree *tree,
		t_node *node, t_node *current)
{
	if (ft_strcmp(current->content[0], "|") == 0)
	{
		handle_pipe_operator(current, node);
	}
	else if (ft_strcmp(current->content[0], node->content[0]) == 0
		|| (starts_with(node->content[0], '<')
			&& starts_with(tree->root->content[0], '>'))
		|| (starts_with(node->content[0], '>')
			&& starts_with(tree->root->content[0], '<')))
		handle_no_root_change(current, node);
	else
	{
		handle_default_redirection(tree, node, current);
	}
}

void	add_operator_node(t_tree *tree, t_node *node)
{
	t_node	*current;

	current = tree->root;
	if (!current)
	{
		tree->root = node;
		return ;
	}
	if (is_redirection(node->content[0]))
	{
		handle_redirection_operator(tree, node, current);
	}
	else
	{
		node->left = tree->root;
		tree->root = node;
	}
}

void	add_non_operator_node(t_tree *tree, t_node *node)
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

void	add_to_tree(t_tree *tree, t_node *node)
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
