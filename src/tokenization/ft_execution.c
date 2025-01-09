/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execution.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 18:44:10 by anikoyan          #+#    #+#             */
/*   Updated: 2025/01/09 10:12:14 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_errno;

void	ft_exec_with_level(t_node *node,
		char ***envp, unsigned short *current_level)
{
	if (!node)
		return ;
	if (node->type == 'O')
		ft_exec_operator(node, envp, current_level);
	else
		ft_exec_command(node, envp, current_level);
}

void	execute_subshell(t_node *node,
		char ***envp, unsigned short *current_level)
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

void	ft_exec_command(t_node *node,
		char ***envp, unsigned short *current_level)
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

void	ft_execute_child_process(t_node *node, char ***envp, int fd)
{
	dup2(fd, STDOUT_FILENO);
	close(fd);
	ft_exec(&(t_tree){node->left}, envp);
	exit(g_errno);
}

void	ft_exec_operator(t_node *node,
		char ***envp, unsigned short *current_level)
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
