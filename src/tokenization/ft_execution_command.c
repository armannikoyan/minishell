/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execution_command.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 16:22:41 by gsimonia          #+#    #+#             */
/*   Updated: 2025/01/13 06:59:31 by anikoyan         ###   ########.fr       */
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
	char			*output;
	char			*tmp;
	bool			inside_quote;
	unsigned short	len;
	unsigned short	i;
	unsigned short	j;
	unsigned short	k;

	if (!node)
		return ;
	i = 0;
	while (node->content[i])
	{
		if (ft_strnstr(node->content[i], "$?", ft_strlen(node->content[i])))
		{
			len = ft_strlen(node->content[i]);
			j = 0;
			inside_quote = false;
			while (node->content[i][j])
			{
				if (node->content[i][j] == '\'')
					inside_quote = !inside_quote;
				if (ft_strncmp(node->content[i] + j, "$?", 2) == 0
					&& !inside_quote)
				{
					len -= 2;
					tmp = ft_itoa(g_errno);
					len += ft_strlen(tmp);
					free(tmp);
				}
				j++;
			}
			output = (char *)malloc(sizeof(char) * (len + 1));
			if (!output)
				exit(EXIT_FAILURE);
			j = 0;
			k = 0;
			inside_quote = false;
			while (node->content[i][j])
			{
				if (node->content[i][j] == '\'')
					inside_quote = !inside_quote;
				if (ft_strncmp(node->content[i] + j, "$?", 2) == 0
					&& !inside_quote)
				{
					tmp = ft_itoa(g_errno);
					ft_strcpy(output + k, tmp);
					k += ft_strlen(tmp);
					free(tmp);
					j += 2;
				}
				else
					output[k++] = node->content[i][j++];
			}
			output[k] = '\0';
			free(node->content[i]);
			node->content[i] = output;
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
		ft_echo(ft_mtx_strlen(node->content), node->content);
	else if (ft_strcmp(node->content[0], "env") == 0)
		ft_env(ft_mtx_strlen(node->content), node->content, envp);
	else if (ft_strcmp(node->content[0], "pwd") == 0)
		ft_pwd(ft_mtx_strlen(node->content), node->content);
	else if (execve(node->content[0], node->content, envp) == -1)
		ft_report_error(node->content[0], ": command not found", 127);
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
