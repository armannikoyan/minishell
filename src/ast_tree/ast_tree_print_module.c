#include <stdio.h>
#include "ast_tree.h"

// Return a name of a node for printing abstract tree
static const char *node_name(t_node_type type)
{
	if (type == NODE_COMMAND)
		return "CMD";
	if (type == NODE_PIPE)
		return "PIPE";
	if (type == NODE_AND)
		return "AND";
	if (type == NODE_OR)
		return "OR";
	if (type == NODE_REDIRECT_IN)
		return "REDIR_IN";
	if (type == NODE_REDIRECT_OUT)
		return "REDIR_OUT";
	if (type == NODE_REDIRECT_APPEND)
		return "REDIR_APPEND";
	if (type == NODE_HEREDOC)
		return "HEREDOC";
	return "UNKNOWN";
}

static int	is_binary(t_node_type type)
{
	return (type == NODE_AND
		|| type == NODE_OR
		|| type == NODE_PIPE);
}

static int	is_redir(t_node_type type)
{
	return (type == NODE_REDIRECT_IN
		|| type == NODE_REDIRECT_OUT
		|| type == NODE_REDIRECT_APPEND
		|| type == NODE_HEREDOC);
}

/*
** Печать argv для командной ноды
*/
static void	print_argv(char **argv)
{
	int	i = 0;

	if (!argv)
		return;
	while (argv[i])
	{
		printf("%s", argv[i]);
		if (argv[i + 1])
			printf(" ");
		i++;
	}
}

/*
** Рекурсивная печать AST с ASCII-разметкой
**
** prefix  — накопленный отступ ("|   ", "    " и т.п.)
** is_last — последний ли это потомок
*/
static void	print_ast_rec(t_ast_node *node, const char *prefix, int is_last)
{
	if (!node)
		return;

	/* Текущая строка */
	printf("%s", prefix);
	printf(is_last ? "`-- " : "|-- ");
	printf("%s", node_name(node->type));

	/* Дополнительные данные ноды */
	if (node->type == NODE_COMMAND)
	{
		printf(": ");
		print_argv(node->u_data.cmd.argv);
	}
	else if (is_redir(node->type))
	{
		printf(": %s", node->u_data.redir.filename);
	}

	printf("\n");

	/* Новый prefix для потомков */
	char	new_prefix[256];
	snprintf(new_prefix, sizeof(new_prefix), "%s%s",
		prefix,
		is_last ? "    " : "|   ");

	if (is_binary(node->type))
	{
		/* левый — не последний, правый — последний */
		print_ast_rec(node->u_data.binary.left, new_prefix, 0);
		print_ast_rec(node->u_data.binary.right, new_prefix, 1);
	}
	else if (is_redir(node->type))
	{
		print_ast_rec(node->u_data.redir.child, new_prefix, 1);
	}
}

/*
** Публичная функция
*/
void	print_ast(t_ast_node *root)
{
	if (!root)
		return;
	print_ast_rec(root, "", 1);
}
