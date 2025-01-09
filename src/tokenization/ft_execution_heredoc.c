/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execution_heredoc.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 16:22:34 by gsimonia          #+#    #+#             */
/*   Updated: 2025/01/09 10:10:59 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_errno;

void	ft_handle_heredoc(t_node *node, char ***envp)
{
	const char	*temp_file;

	temp_file = "/tmp/minishell_heredoc.tmp";
	if (!node || !node->content[1])
	{
		g_errno = 1;
		return ;
	}
	setup_heredoc(node, temp_file);
	if (g_errno == 0)
		handle_heredoc_execution(node, envp, temp_file);
}

void	setup_heredoc(t_node *node, const char *temp_file)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		handle_fork_error();
	if (pid == 0)
		child_heredoc_process(node, temp_file);
	else
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		parent_wait_and_set_status(pid, &status);
		signal(SIGINT, ft_signal_handler);
		signal(SIGQUIT, ft_signal_handler);
		if (g_errno != 0)
			return ;
	}
}

void	handle_heredoc_execution(t_node *node,
		char ***envp, const char *temp_file)
{
	pid_t	pid;
	int		fd;
	int		status;

	fd = open(temp_file, O_RDONLY);
	if (fd == -1)
	{
		handle_open_error();
		return ;
	}
	pid = fork();
	if (pid == -1)
		handle_fork_error();
	if (pid == 0)
		execute_heredoc_child(node, envp, fd);
	else
	{
		close(fd);
		parent_wait_and_set_status(pid, &status);
	}
	unlink(temp_file);
}

void	child_heredoc_process(t_node *node, const char *temp_file)
{
	int		fd;
	char	*line;

	fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		handle_open_error();
	signal(SIGINT, ft_heredoc_signal_handler);
	signal(SIGQUIT, ft_heredoc_signal_handler);
	while (true)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, node->content[1]) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	exit(EXIT_SUCCESS);
}

void	execute_heredoc_child(t_node *node, char ***envp, int fd)
{
	dup2(fd, STDIN_FILENO);
	close(fd);
	ft_exec(&(t_tree){node->left}, envp);
	exit(g_errno);
}
