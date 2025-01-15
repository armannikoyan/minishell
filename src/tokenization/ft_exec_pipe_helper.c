/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_pipe_helper.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 23:44:12 by gsimonia          #+#    #+#             */
/*   Updated: 2025/01/15 23:46:40 by gsimonia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_errno;

void	ft_handle_process_status(int status1, int status2)
{
	if (WIFEXITED(status2))
		g_errno = WEXITSTATUS(status2);
	else if (WIFEXITED(status1))
		g_errno = WEXITSTATUS(status1);
	else
		g_errno = 0;
}

void	ft_wait_processes(pid_t pid1,
		pid_t pid2, int *status1, int *status2)
{
	waitpid(pid1, status1, 0);
	waitpid(pid2, status2, 0);
}

void	ft_close_pipe(int *fd)
{
	close(fd[0]);
	close(fd[1]);
}

void	ft_execute_left_child(t_node *left, char ***envp, int *fd)
{
	close(fd[0]);
	ft_execute_pipe_child(left, envp, fd[1], STDOUT_FILENO);
	close(fd[1]);
	exit(EXIT_SUCCESS);
}

void	ft_execute_right_child(t_node *right, char ***envp, int *fd)
{
	close(fd[1]);
	ft_execute_pipe_child(right, envp, fd[0], STDIN_FILENO);
	close(fd[0]);
	exit(EXIT_SUCCESS);
}
