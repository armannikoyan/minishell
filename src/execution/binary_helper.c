/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 21:26:04 by lvarnach          #+#    #+#             */
/*   Updated: 2026/02/02 21:26:18 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <errno.h>
#include <signal.h>

#include "ast.h"
#include "execution.h"
#include "term_settings.h"
#include "utils.h"

int	abort_pipeline(int prev_fd, int *pipefd)
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

void	exec_child_process(t_ast_node *node, t_p_ctx *ctx,
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
			(clean_all_stacks(g), ast_deletion(g->root), ht_destroy(g->ht));
			exit(1);
		}
		close(pipefd[1]);
		close(pipefd[0]);
	}
	status = execute(node, g->ht, ctx->errnum, g);
	(clean_all_stacks(g), ast_deletion(g->root), ht_destroy(g->ht));
	exit(status);
}

int	run_pipe_step(t_ast_node *node, t_p_ctx *ctx, t_garbage *g)
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

int	run_last_step(t_ast_node *node, t_p_ctx *ctx, t_garbage *g)
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

int	handle_wait_status(int status, int *exit_code)
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
