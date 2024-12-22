/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execution.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 18:44:10 by anikoyan          #+#    #+#             */
/*   Updated: 2024/12/22 23:04:12 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_errno;

void	ft_exec_operator(t_node *node, char **envp);
void	ft_exec_command(t_node *node, char **envp);

void	ft_exec(t_tree *tree, char **envp)
{
	if (!tree || !tree->root)
		return ;
	if (tree->root->type == 'O')
		ft_exec_operator(tree->root, envp);
	else
		ft_exec_command(tree->root, envp);
}

// void	ft_handle_heredoc(t_node *node, char **envp)
// {
// 	// Placeholder for heredoc implementation
// 	fprintf(stderr, "Heredoc not implemented\n");
// 	g_errno = 1;
// }

void	ft_exec_command(t_node *node, char **envp)
{
	pid_t	pid;
	int		status;

	if (!node || !node->content)
		return ;
	// Subshell has level for example (echo 123) has level 1 for each element in the brackets
	// ((echo 123) | cat -e) has level 2 for each element in the brackets of (echo 123) and | and cat -e has 1
	// TODO: Implement subshell execution with level right example of structure ((t_token *)node->content)->subshell_level
	if (node->type == 'S')
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork failed");
			exit(EXIT_FAILURE);
		}
		if (pid == 0)
		{
			ft_exec(&(t_tree){node->left}, envp);
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
		return ;
	}
	// Regular command execution
	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		if (execve(node->content[0], node->content, envp) == -1)
		{
			perror("execve failed");
			exit(EXIT_FAILURE);
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
		close(fd[0]); // Close read end
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
		close(fd[1]); // Close write end
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

	if (!node || !node->left || !node->right)
		return ;
	fd = open(node->right->content[0], O_RDONLY);
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

	if (!node || !node->left || !node->right)
		return ;
	fd = open(node->right->content[0], O_WRONLY | O_CREAT | flags, 0644);
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

void	ft_exec_operator(t_node *node, char **envp)
{
	if (!node)
		return ;
	if (ft_strcmp(node->content[0], "|") == 0)
		ft_handle_pipe(node, envp);
	else if (ft_strcmp(node->content[0], "&&") == 0)
	{
		ft_exec(&(t_tree){node->left}, envp);
		if (g_errno == 0)
			ft_exec(&(t_tree){node->right}, envp);
	}
	else if (ft_strcmp(node->content[0], "||") == 0)
	{
		ft_exec(&(t_tree){node->left}, envp);
		if (g_errno != 0)
			ft_exec(&(t_tree){node->right}, envp);
	}
	else if (ft_strcmp(node->content[0], "<") == 0)
		ft_handle_input_redirection(node, envp);
	else if (ft_strcmp(node->content[0], ">") == 0)
		ft_handle_output_redirection(node, envp, O_TRUNC);
	else if (ft_strcmp(node->content[0], ">>") == 0)
		ft_handle_output_redirection(node, envp, O_APPEND);
	// else if (ft_strcmp(node->content[0], "<<") == 0) {
	// 	ft_handle_heredoc(node, envp);
	// }
	else
	{
		fprintf(stderr, "Unknown operator: %s\n", node->content[0]);
		g_errno = 1;
	}
}
