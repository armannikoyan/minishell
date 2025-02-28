/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execution_command.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 16:22:41 by gsimonia          #+#    #+#             */
/*   Updated: 2025/02/28 15:04:19 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_errno;

int	ft_mtx_strlen(char **mtx)
{
	int	i;

	i = 0;
	while (mtx[i])
		i++;
	return (i);
}

static void	ft_handle_no_file_or_command_error(const char *content)
{
	if (ft_strncmp(content, "./", 2) == 0 || ft_strncmp(content, "/", 1) == 0)
		ft_report_error(content, ": No such file or directory", 127);
	else
		ft_report_error(content, ": command not found", 127);
}

void	ft_check_x_for_errors(t_node *node)
{
	struct stat	statbuf;
	int			status;

	status = stat(node->content[0], &statbuf);
	if (status == 0)
	{
		if (S_ISDIR(statbuf.st_mode))
		{
			if (ft_strncmp(node->content[0], "./", 2) == 0
				|| ft_strncmp(node->content[0], "/", 1) == 0)
				ft_report_error(node->content[0], ": is a directory", 126);
			else
				ft_report_error(node->content[0], ": command not found", 127);
		}
		else if (!(statbuf.st_mode & S_IXUSR))
		{
			if (ft_strncmp(node->content[0], "./", 2) == 0
				|| ft_strncmp(node->content[0], "/", 1) == 0)
				ft_report_error(node->content[0], ": Permission denied", 126);
			else
				ft_report_error(node->content[0], ": command not found", 127);
		}
	}
	else
		ft_handle_no_file_or_command_error(node->content[0]);
}

void	execute_command_in_child(t_node *node, char **envp)
{
	signal(SIGINT, ft_child_signal_handler);
	signal(SIGQUIT, SIG_IGN);
	if (ft_strcmp(node->content[0], "echo") == 0)
		g_errno = ft_echo(ft_mtx_strlen(node->content), node->content);
	else if (ft_strcmp(node->content[0], "env") == 0)
		ft_env(ft_mtx_strlen(node->content), node->content, envp);
	else if (ft_strcmp(node->content[0], "pwd") == 0)
		ft_pwd(ft_mtx_strlen(node->content), node->content);
	else
	{
		if (execve(node->content[0], node->content, envp) == -1)
			ft_check_x_for_errors(node);
		else
			g_errno = errno;
	}
	exit(g_errno);
}

void	execute_command(t_node *node, char ***envp)
{
	// TODO: make execution of the command first do the redirections from left to right then execution of the command itself
	ft_expand_error_code(node);
	ft_quote_removal(node);
	if (ft_strcmp(node->content[0], "cd") == 0)
		ft_cd(ft_mtx_strlen(node->content), node->content, *envp);
	else if (ft_strcmp(node->content[0], "export") == 0)
		ft_export(ft_mtx_strlen(node->content), node->content, envp);
	else if (ft_strcmp(node->content[0], "unset") == 0)
		ft_unset(ft_mtx_strlen(node->content), node->content, *envp);
	else if (ft_strcmp(node->content[0], "exit") == 0)
		ft_exit(ft_mtx_strlen(node->content), node->content);
	else
		handle_fork_and_execute(node, *envp);
}
