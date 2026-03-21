#include <stdlib.h>
#include <stdio.h>

#include "ast.h"
#include "utils.h"
#include "../libs/libft/libft.h"

static t_ast_node	*create_node(t_node_type type)
{
	t_ast_node	*node;

	node = (t_ast_node *)ft_calloc(1, sizeof(t_ast_node));
	if (!node)
	{
		print_error("minishell: calloc", false);
		return (NULL);
	}
	node->type = type;
	node->abstract_type = UNDEF_NODE;
	return (node);
}

t_ast_node	*create_cmd_node(t_node_type type, char **argv)
{
	t_ast_node	*node;

	if (!argv)
		return (NULL);
	node = create_node(type);
	node->abstract_type = CMD_NODE;
	node->u_data.cmd.argv = argv;
	return (node);
}

t_ast_node	*create_binary_node(t_node_type type)
{
	t_ast_node	*node;

	node = create_node(type);
	node->abstract_type = BIN_NODE;
	return (node);
}

t_ast_node	*create_redir_node(t_node_type type, char *filename)
{
	t_ast_node	*node;

	node = create_node(type);
	node->abstract_type = REDIR_NODE;
	node->u_data.redir.filename = filename;
	node->u_data.redir.fd = -1;
	return (node);
}

t_ast_node	*create_subshell_node(t_node_type type, t_ast_node *root)
{
	t_ast_node	*node;

	node = create_node(type);
	node->abstract_type = CMD_NODE;
	node->u_data.subshell.root = root;
	return (node);
}
