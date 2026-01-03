#include <stddef.h>
#include <stdlib.h>

#include "../../includes/ast.h"


#include <stdio.h> // remove

// TODO: Implement the ast building/inserting logic
// Must return root of the tree on each call
t_ast_node	*ast_build(t_ast_node *new_node)
{
	(void)new_node;
	if (new_node->type == NODE_COMMAND)
	{
		printf("-----------------------------\nType: NODE_COMMAND\n");
		for (size_t i = 0; new_node->u_data.cmd.argv[i]; ++i)
			printf("%s\n", new_node->u_data.cmd.argv[i]);
		printf("-----------------------------\n");
	}
	else if (new_node->type == NODE_AND || new_node->type == NODE_OR || new_node->type == NODE_PIPE)
	{
		printf("-----------------------------\nType: NODE_BINARY[%d]\n", new_node->type);
		printf("-----------------------------\n");
	}
	else
	{
		printf("-----------------------------\nType: NODE_REDIR\n");
		printf("%s\n", new_node->u_data.redir.filename);
		printf("-----------------------------\n");
	}
	return (NULL);
}
