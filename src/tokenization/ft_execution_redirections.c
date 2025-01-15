/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execution_redirections.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 16:22:30 by gsimonia          #+#    #+#             */
/*   Updated: 2025/01/15 15:26:48 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_errno;

static int	ft_open_file_for_input_redirection(const char *filepath)
{
	int				fd;

	fd = open(filepath, O_RDONLY);
	if (fd == -1)
	{
		ft_report_error(filepath, ": No such file or directory", 1);
		return (-1);
	}
	if (access(filepath, R_OK) == -1)
	{
		ft_report_error(filepath, ": Permission denied", 1);
		return (-1);
	}
	return (fd);
}

void	ft_handle_input_redirection(t_node *node, char ***envp)
{
	int		fd;
	pid_t	pid;
	int		status;

	if (!node || !node->left || !node->content[1])
		return ;
	fd = ft_open_file_for_input_redirection(node->content[1]);
	if (fd == -1)
		return ;
	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE);
	if (pid == 0)
		ft_execute_input_child_process(node, envp, fd);
	else
	{
		close(fd);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			g_errno = WEXITSTATUS(status);
		else
			g_errno = 1;
	}
}

static int	ft_open_file_for_redirection(const char *filepath, int flags)
{
	int				fd;

	fd = open(filepath, O_WRONLY | O_CREAT | flags, 0644);
	if (fd == -1)
	{
		ft_report_error(filepath, ": No such file or directory", 1);
		return (-1);
	}
	if (access(filepath, W_OK) == -1)
	{
		ft_report_error(filepath, ": Permission denied", 1);
		return (-1);
	}
	return (fd);
}

void	ft_handle_output_redirection(t_node *node, char ***envp, int flags)
{
	int		fd;
	pid_t	pid;
	int		status;

	if (!node || !node->left)
		return ;
	fd = ft_open_file_for_redirection(node->content[1], flags);
	if (fd == -1)
		return ;
	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE);
	if (pid == 0)
		ft_execute_child_process(node, envp, fd);
	else
	{
		close(fd);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			g_errno = WEXITSTATUS(status);
		else
			g_errno = 1;
	}
}

void	ft_execute_input_child_process(t_node *node, char ***envp, int fd)
{
	if (fd == -1)
		exit(g_errno);
	dup2(fd, STDIN_FILENO);
	close(fd);
	ft_exec(&(t_tree){node->left}, envp);
	exit(g_errno);
}
