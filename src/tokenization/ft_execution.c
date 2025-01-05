/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execution.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 18:44:10 by anikoyan          #+#    #+#             */
/*   Updated: 2025/01/04 22:47:05 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_errno;

void	ft_exec_with_level(t_node *node,
			char **envp, unsigned short *current_level);
void	ft_exec_operator(t_node *node,
			char **envp, unsigned short *current_level);
void	ft_exec_command(t_node *node,
			char **envp, unsigned short *current_level);

void	ft_exec(t_tree *tree, char **envp)
{
	static unsigned short	current_level;

	current_level = 0;
	if (!tree || !tree->root)
		return ;
	ft_exec_with_level(tree->root, envp, &current_level);
}

void	ft_exec_with_level(t_node *node,
		char **envp, unsigned short *current_level)
{
	if (!node)
		return ;
	if (node->type == 'O')
		ft_exec_operator(node, envp, current_level);
	else
		ft_exec_command(node, envp, current_level);
}

void	handle_open_error(void)
{
	perror("open failed");
	g_errno = 1;
}

void	parent_wait_and_set_status(pid_t pid, int *status)
{
	waitpid(pid, status, 0);
	if (WIFEXITED(*status))
		g_errno = WEXITSTATUS(*status);
	else
		g_errno = 1;
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

void	setup_heredoc(t_node *node, char **envp, const char *temp_file)
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

void	execute_heredoc_child(t_node *node, char **envp, int fd)
{
	dup2(fd, STDIN_FILENO);
	close(fd);
	ft_exec(&(t_tree){node->left}, envp);
	exit(g_errno);
}

void	handle_heredoc_execution(t_node *node,
		char **envp, const char *temp_file)
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

void	ft_handle_heredoc(t_node *node, char **envp)
{
	const char	*temp_file;

	temp_file = "/tmp/minishell_heredoc.tmp";
	if (!node || !node->content[1])
	{
		g_errno = 1;
		return ;
	}
	setup_heredoc(node, envp, temp_file);
	if (g_errno == 0)
		handle_heredoc_execution(node, envp, temp_file);
}

void	handle_fork_error(void)
{
	perror("fork failed");
	g_errno = 1;
}

void	execute_subshell(t_node *node,
		char **envp, unsigned short *current_level)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		handle_fork_error();
		return ;
	}
	if (pid == 0)
	{
		*current_level = node->subshell_level;
		ft_exec_with_level(node, envp, current_level);
		exit(g_errno);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			g_errno = WEXITSTATUS(status);
		else
			g_errno = 1;
	}
}

void	execute_command_in_child(t_node *node, char **envp)
{
	if (execve(node->content[0], node->content, envp) == -1)
	{
		ft_report_error("command not found: ", node->content[0], 127);
		exit(g_errno);
	}
}

void	execute_command(t_node *node, char **envp)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		handle_fork_error();
		return ;
	}
	if (pid == 0)
	{
		execute_command_in_child(node, envp);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			g_errno = WEXITSTATUS(status);
		else
			g_errno = 1;
	}
}

void	ft_exec_command(t_node *node,
		char **envp, unsigned short *current_level)
{
	if (!node || !node->content)
		return ;
	if (node->subshell_level > *current_level)
	{
		execute_subshell(node, envp, current_level);
		return ;
	}
	execute_command(node, envp);
}

static void	ft_execute_pipe_child(t_node *node,
		char **envp, int fd, int redirect_fd)
{
	dup2(fd, redirect_fd);
	close(fd);
	ft_exec(&(t_tree){node}, envp);
	exit(g_errno);
}

static void	ft_create_pipe(int fd[2])
{
	if (pipe(fd) == -1)
	{
		perror("pipe failed");
		exit(EXIT_FAILURE);
	}
}

static pid_t	ft_fork_process(void)
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

void	ft_handle_pipe(t_node *node, char **envp)
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

static void	ft_execute_input_child_process(t_node *node, char **envp, int fd)
{
	dup2(fd, STDIN_FILENO);
	close(fd);
	ft_exec(&(t_tree){node->left}, envp);
	exit(g_errno);
}

static int	ft_open_file_for_input_redirection(const char *filepath)
{
	int	fd;

	fd = open(filepath, O_RDONLY);
	if (fd == -1)
	{
		perror("open failed");
		g_errno = 1;
	}
	return (fd);
}

void	ft_handle_input_redirection(t_node *node, char **envp)
{
	int		fd;
	pid_t	pid;

	if (!node || !node->left || !node->content[1])
		return ;
	fd = ft_open_file_for_input_redirection(node->content[1]);
	if (fd == -1)
		return ;
	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
		ft_execute_input_child_process(node, envp, fd);
	else
	{
		close(fd);
		waitpid(pid, NULL, 0);
	}
}

static void	ft_execute_child_process(t_node *node, char **envp, int fd)
{
	dup2(fd, STDOUT_FILENO);
	close(fd);
	ft_exec(&(t_tree){node->left}, envp);
	exit(g_errno);
}

static int	ft_open_file_for_redirection(const char *filepath, int flags)
{
	int	fd;

	fd = open(filepath, O_WRONLY | O_CREAT | flags, 0644);
	if (fd == -1)
	{
		perror("open failed");
		g_errno = 1;
	}
	return (fd);
}

void	ft_handle_output_redirection(t_node *node, char **envp, int flags)
{
	int		fd;
	pid_t	pid;

	if (!node || !node->left)
		return ;
	fd = ft_open_file_for_redirection(node->content[1], flags);
	if (fd == -1)
		return ;
	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
		ft_execute_child_process(node, envp, fd);
	else
	{
		close(fd);
		waitpid(pid, NULL, 0);
	}
}

void	ft_exec_operator(t_node *node,
		char **envp, unsigned short *current_level)
{
	if (!node)
		return ;
	if (ft_strcmp(node->content[0], "|") == 0)
		ft_handle_pipe(node, envp);
	else if (ft_strcmp(node->content[0], "&&") == 0)
	{
		ft_exec_with_level(node->left, envp, current_level);
		if (g_errno == 0)
			ft_exec_with_level(node->right, envp, current_level);
	}
	else if (ft_strcmp(node->content[0], "||") == 0)
	{
		ft_exec_with_level(node->left, envp, current_level);
		if (g_errno != 0)
			ft_exec_with_level(node->right, envp, current_level);
	}
	else if (ft_strcmp(node->content[0], "<") == 0)
		ft_handle_input_redirection(node, envp);
	else if (ft_strcmp(node->content[0], ">") == 0)
		ft_handle_output_redirection(node, envp, O_TRUNC);
	else if (ft_strcmp(node->content[0], ">>") == 0)
		ft_handle_output_redirection(node, envp, O_APPEND);
	else if (ft_strcmp(node->content[0], "<<") == 0)
		ft_handle_heredoc(node, envp);
}
