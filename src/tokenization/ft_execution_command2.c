/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execution_command2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 23:27:13 by gsimonia          #+#    #+#             */
/*   Updated: 2025/02/14 14:06:36 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_errno;

void	handle_fork_and_execute(t_node *node, char **envp)
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
		else if (WIFSIGNALED(status))
			g_errno = 128 + WTERMSIG(status);
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
