/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_binary_node.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 02:14:27 by lvarnach          #+#    #+#             */
/*   Updated: 2026/02/02 21:26:29 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#include "ast.h"
#include "execution.h"
#include "hash_table.h"
#include "term_settings.h"
#include "utils.h"
#include "../../libs/libft/libft.h"

int	wait_for_children(t_p_ctx *ctx)
{
	int		status;
	int		exit_code;
	pid_t	pid;

	exit_code = 0;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	while (true)
	{
		pid = waitpid(-1, &status, 0);
		if (pid == -1)
		{
			if (errno == ECHILD)
				break ;
			print_error("minishell: waitpid", false);
			break ;
		}
		if (pid == ctx->last_pid)
			handle_wait_status(status, &exit_code);
	}
	psig_set();
	return (exit_code);
}

int	execute_pipeline(t_ast_node *node, int errnum, t_garbage *g)
{
	t_p_ctx		ctx;
	t_ast_node	*curr;

	ctx.prev_fd = -1;
	ctx.last_pid = -1;
	ctx.ht = g->ht;
	ctx.errnum = errnum;
	curr = node;
	while (curr->type == PIPE_NODE)
	{
		if (run_pipe_step(curr, &ctx, g))
			return (1);
		curr = curr->u_data.binary.right;
	}
	if (run_last_step(curr, &ctx, g))
		return (1);
	return (wait_for_children(&ctx));
}
