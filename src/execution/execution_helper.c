/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 17:53:35 by lvarnach          #+#    #+#             */
/*   Updated: 2026/02/03 01:07:03 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/errno.h>

#include "ast.h"
#include "error_codes.h"
#include "execution.h"
#include "term_settings.h"
#include "utils.h"

static void	del_frame(void *content)
{
	free(content);
}

void	pop_frame(t_list **stack)
{
	t_list	*temp;

	if (!stack || !*stack)
		return ;
	temp = *stack;
	*stack = (*stack)->next;
	ft_lstdelone(temp, del_frame);
}

void	handle_state_zero(t_exec_frame *curr, t_exec_ctx *d, t_ast_node *root)
{
	t_garbage	g;

	g = (t_garbage){.stack = *d->stack, .ht = d->ht,
		.root = root, .next = d->garbage};
	if (curr->node->type == PIPE_NODE)
	{
		*d->status = execute_pipeline(curr->node, *d->status, &g);
		pop_frame(d->stack);
	}
	else if (curr->node->abstract_type == BIN_NODE)
	{
		curr->state = 1;
		push_new_frame(d->stack, curr->node->u_data.binary.left);
	}
	else if (curr->node->abstract_type == REDIR_NODE)
		handle_redir_init(curr, d);
	else
	{
		if (curr->node->type == SUBSHELL_NODE)
			*d->status = execute_subshell(curr->node, *d->status, &g);
		else if (curr->node->abstract_type == CMD_NODE)
			*d->status = execute_command(curr->node, *d->status, &g);
		pop_frame(d->stack);
	}
}

void	handle_state_one(t_exec_frame *curr, t_exec_ctx *d)
{
	int	run_right;

	run_right = 0;
	if (curr->node->type == AND_NODE && *d->status == 0)
		run_right = 1;
	else if (curr->node->type == OR_NODE && *d->status > 0
		&& *d->status < RESERVED_ERROR_CODES)
		run_right = 1;
	if (run_right)
	{
		curr->node = curr->node->u_data.binary.right;
		curr->state = 0;
	}
	else
		pop_frame(d->stack);
}
