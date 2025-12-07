/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 02:57:59 by anikoyan          #+#    #+#             */
/*   Updated: 2025/12/07 21:54:40 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

#include "../../includes/ast.h"

static t_ast_node	*create_node(t_node_type type)
{
	t_ast_node	*node;

	node = (t_ast_node *)malloc(sizeof(t_ast_node));
	if (!node)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	node->type = type;
	return (node);
}

t_ast_node	*create_cmd_node(t_node_type type, char **argv)
{
	t_ast_node	*node;

	node = create_node(type);
	node->u_data.cmd.argv = argv;
	return (node);
}

t_ast_node	*create_binary_node(t_node_type type)
{
	t_ast_node	*node;

	node = create_node(type);
	return (node);
}

t_ast_node	*create_redir_node(t_node_type type, char *filename, int fd)
{
	t_ast_node	*node;

	node = create_node(type);
	node->u_data.redir.filename = filename;
	node->u_data.redir.fd = fd;
	return (node);
}
