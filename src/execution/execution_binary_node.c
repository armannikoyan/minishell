/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_binary_node.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 02:14:27 by lvarnach          #+#    #+#             */
/*   Updated: 2026/01/28 19:57:04 by lvarnach         ###   ########.fr       */
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

static int	abort_pipeline(int prev_fd, int *pipefd)
{
	if (prev_fd != -1)
		close(prev_fd);
	if (pipefd)
	{
		close(pipefd[0]);
		close(pipefd[1]);
	}
	return (1);
}

static void	exec_child_process(t_ast_node *node, t_p_ctx *ctx,
			int *pipefd, t_garbage *g)
{
	int	status;

	if (ctx->prev_fd != -1)
	{
		dup2(ctx->prev_fd, STDIN_FILENO);
		close(ctx->prev_fd);
	}
	if (pipefd)
	{
		if (dup2(pipefd[1], STDOUT_FILENO) == -1)
		{
			print_error("minishell: dup2", false);
			close(pipefd[1]);
			close(pipefd[0]);
			ft_lstclear(&g->stack, free);
			ast_deletion(g->root);
			ht_destroy(g->ht);
			exit(1);
		}
		close(pipefd[1]);
		close(pipefd[0]);
	}
	status = execute(node, g->ht, ctx->errnum, g);

	// CLEANUP
	ft_lstclear(&g->stack, free);
	ast_deletion(g->root);
	ht_destroy(g->ht);
	exit(status);
}

static int	run_pipe_step(t_ast_node *node, t_p_ctx *ctx, t_garbage *g)
{
	int		pipefd[2];
	pid_t	pid;

	if (pipe(pipefd) == -1)
		return (print_error("minishell: pipe", false),
			abort_pipeline(ctx->prev_fd, NULL));
	pid = fork();
	if (pid == -1)
		return (print_error("minishell: fork", false),
			abort_pipeline(ctx->prev_fd, pipefd));
	if (pid == 0)
		exec_child_process(node->u_data.binary.left, ctx, pipefd, g);
	if (ctx->prev_fd != -1)
		close(ctx->prev_fd);
	close(pipefd[1]);
	ctx->prev_fd = pipefd[0];
	return (0);
}

static int	run_last_step(t_ast_node *node, t_p_ctx *ctx, t_garbage *g)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (print_error("minishell: fork", false),
			abort_pipeline(ctx->prev_fd, NULL));
	if (pid == 0)
		exec_child_process(node, ctx, NULL, g);
	ctx->last_pid = pid;
	if (ctx->prev_fd != -1)
		close(ctx->prev_fd);
	return (0);
}

static int	handle_wait_status(int status, int *exit_code)
{
	if (WIFEXITED(status))
		*exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			write(STDOUT_FILENO, "^C\n", 3);
		else if (WTERMSIG(status) == SIGQUIT)
			write(STDOUT_FILENO, "^\\Quit: 3\n", 10);
		*exit_code = 128 + WTERMSIG(status);
	}
	return (0);
}

static int	wait_for_children(t_p_ctx *ctx)
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