/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 17:09:17 by lvarnach          #+#    #+#             */
/*   Updated: 2026/01/28 17:30:17 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "ast.h"
#include "hash_table.h"
#include "utils.h"
#include "execution.h"
#include "term_settings.h"
#include "../../libs/libft/libft.h"

static void	free_exec_frame(void *content)
{
	t_exec_frame	*frame;

	frame = (t_exec_frame *)content;
	if (frame && frame->saved_fd != -1)
		close(frame->saved_fd);
	free(frame);
}

void	clean_all_stacks(t_garbage *g)
{
	t_garbage	*curr;

	curr = g;
	while (curr)
	{
		ft_lstclear(&curr->stack, free_exec_frame);
		curr = curr->next;
	}
}

int	execute_subshell(t_ast_node *node, int errnum, t_garbage *g)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return (print_error("minishell: fork", false), 1);
	if (pid)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		status = handle_child_exit(pid);
		psig_set();
		return (status);
	}
	status = execute(node->u_data.subshell.root, g->ht, errnum, g);

	clean_all_stacks(g);
	ast_deletion(g->root);
	ht_destroy(g->ht);

	exit(status);
}

int	push_new_frame(t_list **stack, t_ast_node *node)
{
	t_exec_frame	*frame;
	t_list			*new_node;

	if (!node)
		return (0);
	frame = malloc(sizeof(t_exec_frame));
	if (!frame)
		return (1);
	frame->node = node;
	frame->state = 0;
	frame->saved_fd = -1;
	frame->target_fd = -1;
	new_node = ft_lstnew(frame);
	if (!new_node)
		return (free(frame), 1);
	ft_lstadd_front(stack, new_node);
	return (0);
}

void	handle_redir_init(t_exec_frame *frame, t_exec_ctx *ctx, t_garbage *g)
{
	// We pass 'g' to setup_redirection in case it needs to fork (HereDoc)
	if (setup_redirection(frame->node, ctx->ht,
			&frame->saved_fd, &frame->target_fd, g))
	{
		*ctx->status = 1;
		pop_frame(ctx->stack);
	}
	else
	{
		frame->state = 2;
		push_new_frame(ctx->stack, frame->node->u_data.redir.child);
	}
}

int	execute(t_ast_node *node, t_hash_table *ht, int errnum, t_garbage *g)
{
	t_list			*stack;
	t_exec_ctx		ctx;
	t_exec_frame	*curr;

	if (!node)
		return (0);
	stack = NULL;
	if (push_new_frame(&stack, node))
		return (1);
	ctx.stack = &stack;
	ctx.ht = ht;
	ctx.status = &errnum;
	ctx.garbage = g;
	while (stack)
	{
		curr = (t_exec_frame *)stack->content;
		if (curr->state == 0)
			handle_state_zero(curr, &ctx, g->root);
		else if (curr->state == 1)
			handle_state_one(curr, &ctx);
		else if (curr->state == 2)
			(cleanup_redirection(curr->node, curr->saved_fd,
					curr->target_fd), pop_frame(&stack));
	}
	ft_lstclear(&stack, free);
	return (errnum);
}