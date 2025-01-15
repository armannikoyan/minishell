/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exectuion_pipe.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 16:22:44 by gsimonia          #+#    #+#             */
/*   Updated: 2025/01/15 23:46:14 by gsimonia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_errno;

static int	ft_validate_pipe_input(t_node *node)
{
	return (node && node->left && node->right);
}

void	ft_handle_pipe(t_node *node, char ***envp)
{
	int		fd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status1;
	int		status2;

	if (!ft_validate_pipe_input(node))
		return ;
	ft_create_pipe(fd);
	pid1 = ft_fork_process();
	if (pid1 == 0)
		ft_execute_left_child(node->left, envp, fd);
	pid2 = ft_fork_process();
	if (pid2 == 0)
		ft_execute_right_child(node->right, envp, fd);
	ft_close_pipe(fd);
	ft_wait_processes(pid1, pid2, &status1, &status2);
	ft_handle_process_status(status1, status2);
}

void	ft_execute_pipe_child(t_node *node, char ***envp, int fd,
		int redirect_fd)
{
	dup2(fd, redirect_fd);
	close(fd);
	ft_exec(&(t_tree){node}, envp);
	exit(g_errno);
}

void	ft_create_pipe(int fd[2])
{
	if (pipe(fd) == -1)
	{
		perror("pipe failed");
		exit(EXIT_FAILURE);
	}
}

pid_t	ft_fork_process(void)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		exit(EXIT_FAILURE);
	}
	return (pid);
}
