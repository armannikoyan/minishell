/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_syntax_tree.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 12:47:00 by anikoyan          #+#    #+#             */
/*   Updated: 2025/01/15 23:32:37 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	starts_with(const char *str, char c)
{
	return (ft_strncmp(str, &c, 1) == 0);
}

int	is_redirection(const char *content)
{
	return (starts_with(content, '<') || starts_with(content, '>'));
}

void	handle_pipe_operator(t_node *current, t_node *node)
{
	if (!current->right)
		current->right = node;
	else
	{
		node->left = current->right;
		current->right = node;
	}
}

void	handle_no_root_change(t_node *current, t_node *node)
{
	node->left = current->left;
	node->right = current->right;
	current->left = node;
	current->right = NULL;
}

void	handle_default_redirection(t_tree *tree,
		t_node *node, t_node *current)
{
	node->left = current;
	tree->root = node;
}
