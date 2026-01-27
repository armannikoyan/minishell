/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_node_deletion.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 17:53:30 by lvarnach          #+#    #+#             */
/*   Updated: 2026/01/27 17:54:12 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>

#include "ast.h"
#include "utils.h"

static void	command_node_deletion(t_ast_node *node)
{
	if (node->type == COMMAND_NODE)
		free_split(node->u_data.cmd.argv);
	else if (node->type == SUBSHELL_NODE)
		ast_deletion(node->u_data.subshell.root);
	free(node);
}

static void	binary_node_deletion(t_ast_node *node)
{
	ast_deletion(node->u_data.binary.left);
	ast_deletion(node->u_data.binary.right);
	free(node);
}

static void	redir_node_deletion(t_ast_node *node)
{
	ast_deletion(node->u_data.redir.child);
	free(node->u_data.redir.filename);
	free(node);
}

void	ast_deletion(t_ast_node *root)
{
	if (root == NULL)
		return ;
	if (root->abstract_type == CMD_NODE)
		return (command_node_deletion(root));
	else if (root->abstract_type == BIN_NODE)
		return (binary_node_deletion(root));
	else if (root->abstract_type == REDIR_NODE)
		return (redir_node_deletion(root));
}
