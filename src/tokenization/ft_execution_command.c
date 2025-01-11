/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execution_command.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 16:22:41 by gsimonia          #+#    #+#             */
/*   Updated: 2025/01/11 10:53:27 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_errno;

static void	handle_fork_and_execute(t_node *node, char **envp)
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
		execute_command_in_child(node, envp);
	else
	{
		signal(SIGINT, ft_parent_child_signal_handler);
		signal(SIGQUIT, SIG_IGN);
		waitpid(pid, &status, 0);
		signal(SIGINT, ft_signal_handler);
		signal(SIGQUIT, ft_signal_handler);
		if (WIFEXITED(status))
			g_errno = WEXITSTATUS(status);
		else
			g_errno = 1;
	}
}

void	ft_expand_error_code(t_node *node)
{
	unsigned short	i;

	if (!node)
		return ;
	i = 0;
	while (node->content[i])
	{
		if (ft_strcmp(node->content[i], "$?") == 0)
		{
			free(node->content[i]);
			node->content[i] = ft_itoa(g_errno);
		}
		i++;
	}
}

int	ft_mtx_strlen(char **mtx)
{
	int	i;

	i = 0;
	while (mtx[i])
		i++;
	return (i);
}

void	execute_command_in_child(t_node *node, char **envp)
{
	signal(SIGINT, ft_child_signal_handler);
	signal(SIGQUIT, SIG_IGN);
	if (ft_strcmp(node->content[0], "echo") == 0)
		g_errno = ft_echo(ft_mtx_strlen(node->content), node->content);
	else if (ft_strcmp(node->content[0], "env") == 0)
		g_errno = ft_env(ft_mtx_strlen(node->content), node->content, envp);
	else if (ft_strcmp(node->content[0], "pwd") == 0)
		g_errno = ft_pwd(ft_mtx_strlen(node->content), node->content);
	else if (execve(node->content[0], node->content, envp) == -1)
		ft_report_error("command not found: ", node->content[0], 127);
	exit(g_errno);
}

void	execute_command(t_node *node, char ***envp)
{
	char			*tmp;
	unsigned short	i;

	i = 0;
	while ((*envp)[i] && ft_strncmp((*envp)[i], "_=", 2) != 0)
		i++;
	tmp = (*envp)[i];
	(*envp)[i] = ft_strjoin("_=", node->content[0]);
	ft_expand_error_code(node);
	if (ft_strcmp(node->content[0], "cd") == 0)
		ft_cd(ft_mtx_strlen(node->content), node->content, *envp);
	else if (ft_strcmp(node->content[0], "export") == 0)
		ft_export(ft_mtx_strlen(node->content), node->content, envp);
	else if (ft_strcmp(node->content[0], "unset") == 0)
		ft_unset(ft_mtx_strlen(node->content), node->content, *envp);
	else if (ft_strcmp(node->content[0], "exit") == 0)
		g_errno = ft_exit(ft_mtx_strlen(node->content), node->content);
	else
		handle_fork_and_execute(node, *envp);
	i = 0;
	while ((*envp)[i] && ft_strncmp((*envp)[i], "_=", 2) != 0)
		i++;
	free((*envp)[i]);
	(*envp)[i] = tmp;
}
