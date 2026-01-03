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
	if (argv)
		node->u_data.cmd.argv = argv;
	else
		node->u_data.cmd.argv = NULL;
	return (node);
}

t_ast_node	*create_binary_node(t_node_type type)
{
	t_ast_node	*node;

	node = create_node(type);
	node->u_data.binary.left = NULL;
	node->u_data.binary.right = NULL;
	return (node);
}

t_ast_node	*create_redir_node(t_node_type type, char *filename, int fd)
{
	t_ast_node	*node;

	node = create_node(type);
	if (node->u_data.redir.filename)
		node->u_data.redir.filename = filename;
	else
		node->u_data.redir.filename = NULL;
	node->u_data.redir.fd = fd;
	return (node);
}
