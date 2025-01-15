/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execution_command.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 16:22:41 by gsimonia          #+#    #+#             */
/*   Updated: 2025/01/15 15:03:57 by anikoyan         ###   ########.fr       */
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

static unsigned short	calculate_new_length(const char *str, int g_errno)
{
	unsigned short	len;
	unsigned short	j;
	bool			inside_quote;
	char			*tmp;

	len = ft_strlen(str);
	j = 0;
	inside_quote = false;
	while (str[j])
	{
		if (str[j] == '\'')
			inside_quote = !inside_quote;
		if (ft_strncmp(str + j, "$?", 2) == 0 && !inside_quote)
		{
			len -= 2;
			tmp = ft_itoa(g_errno);
			len += ft_strlen(tmp);
			free(tmp);
		}
		j++;
	}
	return (len);
}

static void	replace_error_codes(const char *input, char *output, int g_errno)
{
	unsigned short	j;
	unsigned short	k;
	bool			inside_quote;
	char			*tmp;

	j = 0;
	k = 0;
	inside_quote = false;
	while (input[j])
	{
		if (input[j] == '\'')
			inside_quote = !inside_quote;
		if (ft_strncmp(input + j, "$?", 2) == 0 && !inside_quote)
		{
			tmp = ft_itoa(g_errno);
			ft_strcpy(output + k, tmp);
			k += ft_strlen(tmp);
			free(tmp);
			j += 2;
		}
		else
			output[k++] = input[j++];
	}
	output[k] = '\0';
}

static void	process_node_content(t_node *node, int g_errno)
{
	unsigned short	len;
	char			*output;
	unsigned short	i;

	i = 0;
	while (node->content[i])
	{
		if (ft_strnstr(node->content[i], "$?", ft_strlen(node->content[i])))
		{
			len = calculate_new_length(node->content[i], g_errno);
			output = (char *)malloc(sizeof(char) * (len + 1));
			if (!output)
				exit(EXIT_FAILURE);
			replace_error_codes(node->content[i], output, g_errno);
			free(node->content[i]);
			node->content[i] = output;
		}
		i++;
	}
}

void	ft_expand_error_code(t_node *node)
{
	if (!node)
		return ;
	process_node_content(node, g_errno);
}

int	ft_mtx_strlen(char **mtx)
{
	int	i;

	i = 0;
	while (mtx[i])
		i++;
	return (i);
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
	{
		if (ft_strncmp(node->content[0], "./", 2) == 0
			|| ft_strncmp(node->content[0], "/", 1) == 0)
			ft_report_error(node->content[0], ": No such file or directory", 127);
		else
			ft_report_error(node->content[0], ": command not found", 127);
	}
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
