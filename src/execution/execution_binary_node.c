/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_binary_node.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 02:14:27 by lvarnach          #+#    #+#             */
/*   Updated: 2026/01/28 02:17:43 by lvarnach         ###   ########.fr       */
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

/*
 ** CLEANUP HANDLER
 ** If fork fails, we must close open FDs to trigger SIGPIPE
 */
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

/*
 ** Pipeline execution the bash way:
 ** - Fork each process in sequence
 ** - Don't track all PIDs, only the last one matters for exit status
 ** - The pipeline works through pipe chaining, not waiting
 */
int	execute_pipeline(t_ast_node *node, t_hash_table *ht, int errnum)
{
	int			pipefd[2];
	int			prev_fd;
	t_ast_node	*curr;
	pid_t		pid;
	pid_t		last_pid;
	int			status;
	int			exit_code;
	char		buf[50];
	int			len;
	int			sig;

	prev_fd = -1;
	curr = node;
	last_pid = -1;
	while (curr->type == PIPE_NODE)
	{
		if (pipe(pipefd) == -1)
		{
			print_error("minishell: pipe", false);
			return (abort_pipeline(prev_fd, NULL));
		}
		pid = fork();
		if (pid == -1)
		{
			print_error("minishell: fork", false);
			return (abort_pipeline(prev_fd, pipefd));
		}
		if (pid == 0)
		{
			if (prev_fd != -1)
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}
			dup2(pipefd[1], STDOUT_FILENO);
			close(pipefd[1]);
			close(pipefd[0]);
			exit(execute(curr->u_data.binary.left, ht, errnum));
		}
		if (prev_fd != -1)
			close(prev_fd);
		close(pipefd[1]);
		prev_fd = pipefd[0];
		curr = curr->u_data.binary.right;
	}
	pid = fork();
	if (pid == -1)
	{
		print_error("minishell: fork", false);
		return (abort_pipeline(prev_fd, NULL));
	}
	if (pid == 0)
	{
		if (prev_fd != -1)
		{
			dup2(prev_fd, STDIN_FILENO);
			close(prev_fd);
		}
		exit(execute(curr, ht, errnum));
	}
	last_pid = pid;
	if (prev_fd != -1)
		close(prev_fd);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	exit_code = 0;
	while (1)
	{
		pid = waitpid(-1, &status, 0);
		if (pid == -1)
		{
			if (errno == ECHILD)
				break ;
			print_error("minishell: waitpid", false);
			break ;
		}
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
				exit_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				sig = WTERMSIG(status);
				if (sig == SIGINT)
					write(STDOUT_FILENO, "^C\n", 3);
				else if (sig == SIGQUIT)
				{
					len = 0;
					ft_memcpy(buf, "^\\Quit: signum: ", 16);
					len = 16;
					buf[len++] = sig - '0';
					buf[len++] = '\n';
					write(STDOUT_FILENO, buf, len);
				}
				exit_code = 128 + sig;
			}
		}
	}
	psig_set();
	return (exit_code);
}
