/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_syntax_tree.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 12:47:00 by anikoyan          #+#    #+#             */
/*   Updated: 2025/01/14 12:43:58 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	starts_with(const char *str, char c)
{
	return (ft_strncmp(str, &c, 1) == 0);
}

static int	is_redirection(const char *content)
{
	return (starts_with(content, '<') || starts_with(content, '>'));
}

static void	handle_pipe_operator(t_node *current, t_node *node)
{
	if (!current->right)
		current->right = node;
	else
	{
		node->left = current->right;
		current->right = node;
	}
}

static void	handle_same_operator(t_node *current, t_node *node)
{
	node->left = current->left;
	node->right = current->right;
	current->left = node;
}

static void	handle_input_with_output_root(t_node *current, t_node *node)
{
	node->left = current->left;
	node->right = current->right;
	current->left = node;
	current->right = NULL;
}

static void	handle_default_redirection(t_tree *tree,
		t_node *node, t_node *current)
{
	node->left = current;
	tree->root = node;
}

static void	handle_redirection_operator(t_tree *tree,
		t_node *node, t_node *current)
{
	if (ft_strcmp(current->content[0], "|") == 0)
	{
		handle_pipe_operator(current, node);
	}
	else if (ft_strcmp(current->content[0], node->content[0]) == 0)
	{
		handle_same_operator(current, node);
	}
	else if (starts_with(node->content[0], '<')
		&& starts_with(tree->root->content[0], '>'))
	{
		handle_input_with_output_root(current, node);
	}
	else
	{
		handle_default_redirection(tree, node, current);
	}
}

static void	add_operator_node(t_tree *tree, t_node *node)
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
