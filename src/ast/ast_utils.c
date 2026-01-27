/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 21:10:06 by lvarnach          #+#    #+#             */
/*   Updated: 2026/01/27 21:10:26 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "error_codes.h"
#include "utils.h"

void	print_syntax_error(t_ast_node *node, int *errnum)
{
	if (!node)
		return ;
	*errnum = SYNTAX_ERROR;
	print_error("minishell: syntax error near unexpected token `", true);
	if (node->abstract_type == BIN_NODE || node->abstract_type == REDIR_NODE)
		print_error(get_type(node->type), true);
	else if (node->type == COMMAND_NODE && node->u_data.cmd.argv)
		print_error(node->u_data.cmd.argv[0], true);
	else if (node->type == SUBSHELL_NODE)
		print_syntax_error(node->u_data.subshell.root, errnum);
	print_error("'\n", true);
}

int	get_precedence(t_node_type type)
{
	if (type == PIPE_NODE)
		return (2);
	if (type == AND_NODE || type == OR_NODE)
		return (1);
	return (0);
}
