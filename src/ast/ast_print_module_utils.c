/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_print_module_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 17:54:28 by lvarnach          #+#    #+#             */
/*   Updated: 2026/01/27 19:17:30 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#include "ast.h"

// Return a name of a node for printing abstract tree
static const char	*get_node_name(t_node_type type)
{
	if (type == COMMAND_NODE)
		return ("CMD");
	if (type == PIPE_NODE)
		return ("PIPE");
	if (type == AND_NODE)
		return ("AND");
	if (type == OR_NODE)
		return ("OR");
	if (type == REDIRECT_IN_NODE)
		return ("REDIR_IN");
	if (type == REDIRECT_OUT_NODE)
		return ("REDIR_OUT");
	if (type == REDIRECT_APPEND_NODE)
		return ("REDIR_APPEND");
	if (type == HEREDOC_NODE)
		return ("HEREDOC");
	if (type == SUBSHELL_NODE)
		return ("SUBSHELL");
	return ("UNKNOWN");
}

// Prints argv for command node
static void	print_argv(char **argv)
{
	int	i;

	i = 0;
	printf(": ");
	if (!argv)
		return ;
	while (argv[i])
	{
		printf("%s", argv[i]);
		if (argv[i + 1])
			printf(" ");
		i++;
	}
}

// Prints accumulated prefix
static void	print_prefix(int depth, int *has_pipe)
{
	int	i;

	i = 0;
	while (i < depth)
	{
		if (has_pipe[i])
			printf("|   ");
		else
			printf("    ");
		i++;
	}
}

// Recursive printing of abstract tree with ASCII-marking
static void	print_ast_rec(t_ast_node *node, int depth, int *has_pipe,
	int is_last)
{
	if (!node)
		return ;
	print_prefix(depth, has_pipe);
	if (is_last)
		printf("`-- ");
	else
		printf("|-- ");
	printf("%s", get_node_name(node->type));
	if (node->type == COMMAND_NODE)
		print_argv(node->u_data.cmd.argv);
	else if (node->abstract_type == REDIR_NODE)
		printf(": %s", node->u_data.redir.filename);
	printf("\n");
	has_pipe[depth] = !is_last;
	if (node->abstract_type == BIN_NODE)
	{
		print_ast_rec(node->u_data.binary.left, depth + 1, has_pipe, 0);
		print_ast_rec(node->u_data.binary.right, depth + 1, has_pipe, 1);
	}
	else if (node->abstract_type == REDIR_NODE)
		print_ast_rec(node->u_data.redir.child, depth + 1, has_pipe, 1);
}

// Prints abstract syntax tree
void	print_ast(t_ast_node *root)
{
	int	has_pipe[64];
	int	i;

	if (!root)
		return ;
	i = 0;
	while (i < 64)
		has_pipe[i++] = 0;
	print_ast_rec(root, 0, has_pipe, 1);
}
