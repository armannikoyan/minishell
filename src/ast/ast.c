/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 19:17:45 by lvarnach          #+#    #+#             */
/*   Updated: 2026/01/27 21:25:43 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#include "utils.h"
#include "ast.h"

#include "error_codes.h"

static t_ast_node	*root_is_command_node(t_ast_node *node, t_ast_node *root,
		int *errnum)
{
	if (node->abstract_type == BIN_NODE)
	{
		node->u_data.binary.left = root;
		return (node);
	}
	if (node->abstract_type == REDIR_NODE)
	{
		node->u_data.redir.child = root;
		return (node);
	}
	*errnum = SYNTAX_ERROR;
	print_syntax_error(node, errnum);
	return (NULL);
}

static t_ast_node	*root_is_binary_node(t_ast_node *node, t_ast_node *root,
		int *errnum)
{
	if (node->abstract_type == CMD_NODE || node->abstract_type == REDIR_NODE)
	{
		if (root->u_data.binary.right != NULL)
		{
			root->u_data.binary.right = ast_build(node,
					root->u_data.binary.right, errnum);
			return (root);
		}
		root->u_data.binary.right = node;
		return (root);
	}
	if (node->abstract_type == BIN_NODE)
	{
		if (get_precedence(node->type) > get_precedence(root->type))
		{
			root->u_data.binary.right = ast_build(node,
					root->u_data.binary.right, errnum);
			return (root);
		}
		node->u_data.binary.left = root;
		return (node);
	}
	*errnum = SYNTAX_ERROR;
	print_syntax_error(node, errnum);
	return (NULL);
}

static t_ast_node	*root_redir_node_redir_or_binary(t_ast_node *node,
	t_ast_node *root)
{
	t_ast_node	*iter;

	iter = root;
	if (node->abstract_type == REDIR_NODE)
	{
		while (iter->u_data.redir.child != NULL
			&& iter->u_data.redir.child->abstract_type == REDIR_NODE)
			iter = iter->u_data.redir.child;
		if (iter->u_data.redir.child == NULL)
		{
			iter->u_data.redir.child = node;
			return (root);
		}
		node->u_data.redir.child = iter->u_data.redir.child;
		iter->u_data.redir.child = node;
		return (root);
	}
	if (node->abstract_type == BIN_NODE)
	{
		node->u_data.binary.left = root;
		return (node);
	}
	return (NULL);
}

static t_ast_node	*root_is_redir_node(t_ast_node *node, t_ast_node *root,
		int *errnum)
{
	t_ast_node	*iter;

	iter = root;
	if (node->abstract_type == REDIR_NODE || node->abstract_type == BIN_NODE)
		return (root_redir_node_redir_or_binary(node, root));
	if (node->abstract_type == CMD_NODE)
	{
		while (iter->u_data.redir.child != NULL
			&& iter->u_data.redir.child->abstract_type == REDIR_NODE)
			iter = iter->u_data.redir.child;
		if (iter->u_data.redir.child == NULL)
		{
			iter->u_data.redir.child = node;
			return (root);
		}
	}
	*errnum = SYNTAX_ERROR;
	print_syntax_error(node, errnum);
	return (NULL);
}

// Takes a head node of abstract tree and a new node,
// and returns an updated ast tree.
// If a syntaxis error occurred while updating tree, returns NULL
t_ast_node	*ast_build(t_ast_node *new_node, t_ast_node *root, int *errnum)
{
	if (root == NULL)
		return (new_node);
	if (root->abstract_type == CMD_NODE)
		return (root_is_command_node(new_node, root, errnum));
	if (root->abstract_type == BIN_NODE)
		return (root_is_binary_node(new_node, root, errnum));
	if (root->abstract_type == REDIR_NODE)
		return (root_is_redir_node(new_node, root, errnum));
	*errnum = SYNTAX_ERROR;
	print_syntax_error(new_node, errnum);
	return (NULL);
}
