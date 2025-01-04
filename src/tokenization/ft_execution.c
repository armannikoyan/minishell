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

void	ft_exec_with_level(t_node *node, char **envp, unsigned short *current_level);
void	ft_exec_operator(t_node *node, char **envp, unsigned short *current_level);
void	ft_exec_command(t_node *node, char **envp, unsigned short *current_level);


void	ft_exec(t_tree *tree, char **envp)
{
	static unsigned short	current_level = 0;

	if (!tree || !tree->root)
		return ;

	ft_exec_with_level(tree->root, envp, &current_level);
}

void	ft_exec_with_level(t_node *node, char **envp, unsigned short *current_level)
{
	if (!node)
		return ;
	if (node->type == 'O')
		ft_exec_operator(node, envp, current_level);
	else
		ft_exec_command(node, envp, current_level);
}

void	ft_handle_heredoc(t_node *node, char **envp)
{
	// TODO: handle heredoc signals
	pid_t	pid;
	int		fd;
	int		status;
	char	*line;
	char	*temp_file = "/tmp/minishell_heredoc.tmp";

	if (!node || !node->content[1])
	{
		g_errno = 1;
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			perror("open failed");
			exit(EXIT_FAILURE);
		}
		signal(SIGINT, ft_heredoc_signal_handler);
		signal(SIGQUIT, ft_heredoc_signal_handler);
		while (true)
		{
			line = readline("> ");
			if (!line)
				break;
			if (ft_strcmp(line, node->content[1]) == 0)
			{
				free(line);
				break;
			}
			write(fd, line, ft_strlen(line));
			write(fd, "\n", 1);
			free(line);
		}
		close(fd);
		exit(EXIT_SUCCESS);
	}
	else
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		waitpid(pid, &status, 0);
		signal(SIGINT, ft_signal_handler);
		signal(SIGQUIT, ft_signal_handler);
		if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
		{
			g_errno = status;
			return ;
		}
		fd = open(temp_file, O_RDONLY);
		if (fd == -1)
		{
			perror("open failed");
			g_errno = 1;
			return ;
		}
		pid = fork();
		if (pid == -1)
		{
			perror("fork failed");
			exit(EXIT_FAILURE);
		}
		if (pid == 0)
		{
			dup2(fd, STDIN_FILENO);
			close(fd);
			ft_exec(&(t_tree){node->left}, envp);
			exit(g_errno);
		}
		else
		{
			close(fd);
			waitpid(pid, &status, 0);
			if (WIFEXITED(status))
				g_errno = WEXITSTATUS(status);
			else
				g_errno = 1;
		}
		unlink(temp_file);
	}
}

void ft_exec_command(t_node *node, char **envp, unsigned short *current_level)
{
	pid_t pid;
	int status;

	if (!node || !node->content)
		return;

	if (node->subshell_level > *current_level)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork failed");
			g_errno = 1;
			return;
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
			return;
		}
	}

	// Regular command execution
	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		g_errno = 1;
		return;
	}

	if (pid == 0)
	{
		if (execve(node->content[0], node->content, envp) == -1)
		{
			ft_report_error("command not found: ", node->content[0], 127);
			exit(g_errno);
		}
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

void	ft_handle_pipe(t_node *node, char **envp)
{
	int		fd[2];
	pid_t	pid1;
	pid_t	pid2;

	if (!node || !node->left || !node->right)
		return ;
	if (pipe(fd) == -1)
	{
		perror("pipe failed");
		exit(EXIT_FAILURE);
	}
	pid1 = fork();
	if (pid1 == -1)
	{
		perror("fork failed");
		exit(EXIT_FAILURE);
	}
	if (pid1 == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		ft_exec(&(t_tree){node->left}, envp);
		exit(g_errno);
	}
	pid2 = fork();
	if (pid2 == -1)
	{
		perror("fork failed");
		exit(EXIT_FAILURE);
	}
	if (pid2 == 0)
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		ft_exec(&(t_tree){node->right}, envp);
		exit(g_errno);
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
}

void	ft_handle_input_redirection(t_node *node, char **envp)
{
	int		fd;
	pid_t	pid;

	if (!node || !node->left || !node->content[1])
		return ;
	fd = open(node->content[1], O_RDONLY);
	if (fd == -1)
	{
		perror("open failed");
		g_errno = 1;
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		dup2(fd, STDIN_FILENO);
		close(fd);
		ft_exec(&(t_tree){node->left}, envp);
		exit(g_errno);
	}
	else
	{
		close(fd);
		waitpid(pid, NULL, 0);
	}
}

void	ft_handle_output_redirection(t_node *node, char **envp, int flags)
{
	int		fd;
	pid_t	pid;

	if (!node || !node->left)
		return ;
	fd = open(node->content[1], O_WRONLY | O_CREAT | flags, 0644);
	if (fd == -1)
	{
		perror("open failed");
		g_errno = 1;
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		dup2(fd, STDOUT_FILENO);
		close(fd);
		ft_exec(&(t_tree){node->left}, envp);
		exit(g_errno);
	}
	else
	{
		close(fd);
		waitpid(pid, NULL, 0);
	}
}

void	ft_exec_operator(t_node *node, char **envp, unsigned short *current_level)
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
