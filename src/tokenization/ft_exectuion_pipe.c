/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exectuion_pipe.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 16:22:44 by gsimonia          #+#    #+#             */
/*   Updated: 2025/01/13 02:11:58 by gsimonia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_errno;

void	ft_handle_pipe(t_node *node, char ***envp)
{
	int		fd[2];
	pid_t	pid1;
	pid_t	pid2;

	if (!node || !node->left || !node->right)
		return ;
	ft_create_pipe(fd);
	pid1 = ft_fork_process();
	if (pid1 == 0)
	{
		close(fd[0]);
		ft_execute_pipe_child(node->left, envp, fd[1], STDOUT_FILENO);
	}
	pid2 = ft_fork_process();
	if (pid2 == 0)
	{
		close(fd[1]);
		ft_execute_pipe_child(node->right, envp, fd[0], STDIN_FILENO);
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
}

void	ft_execute_pipe_child(t_node *node,
		char ***envp, int fd, int redirect_fd)
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
