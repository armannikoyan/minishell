#include <stdlib.h>
#include <stdio.h>

#include "../../includes/ast_tree.h"

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
	node->u_data.cmd.argv = NULL;
	node->u_data.binary.left = NULL;
	node->u_data.binary.right = NULL;
	node->u_data.redir.child = NULL;
	node->u_data.redir.filename = NULL;
	node->u_data.redir.fd = -1;
	return (node);
}

t_ast_node	*create_cmd_node(t_node_type type, char **argv)
{
	t_ast_node	*node;

	node = create_node(type);
	if (argv)
		node->u_data.cmd.argv = argv;
	return (node);
}

t_ast_node	*create_binary_node(t_node_type type)
{
	t_ast_node	*node;

	node = create_node(type);
	return (node);
}

t_ast_node	*create_redir_node(t_node_type type, char *filename)
{
	t_ast_node	*node;

	node = create_node(type);
	if (filename)
		node->u_data.redir.filename = filename;
	return (node);
}
