/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exectuion_pipe.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 16:22:44 by gsimonia          #+#    #+#             */
/*   Updated: 2025/01/15 20:39:22 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_errno;

static void	ft_handle_process_status(int status1, int status2)
{
	if (WIFEXITED(status2))
		g_errno = WEXITSTATUS(status2);
	else if (WIFEXITED(status1))
		g_errno = WEXITSTATUS(status1);
	else
		g_errno = 0;
}

static void	ft_wait_processes(pid_t pid1,
		pid_t pid2, int *status1, int *status2)
{
	waitpid(pid1, status1, 0);
	waitpid(pid2, status2, 0);
}

static void	ft_close_pipe(int *fd)
{
	close(fd[0]);
	close(fd[1]);
}

static void	ft_execute_left_child(t_node *left, char ***envp, int *fd)
{
	close(fd[0]);
	ft_execute_pipe_child(left, envp, fd[1], STDOUT_FILENO);
	close(fd[1]);
	exit(EXIT_SUCCESS);
}

static void	ft_execute_right_child(t_node *right, char ***envp, int *fd)
{
	close(fd[1]);
	ft_execute_pipe_child(right, envp, fd[0], STDIN_FILENO);
	close(fd[0]);
	exit(EXIT_SUCCESS);
}

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
