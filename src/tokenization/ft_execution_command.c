/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execution_command.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 16:22:41 by gsimonia          #+#    #+#             */
/*   Updated: 2025/01/12 07:41:28 by gsimonia         ###   ########.fr       */
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
	// ISSUE: make so that $?HOME -> g_errnoHOME
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
		ft_echo(ft_mtx_strlen(node->content), node->content);
	else if (ft_strcmp(node->content[0], "env") == 0)
		ft_env(ft_mtx_strlen(node->content), node->content, envp);
	else if (ft_strcmp(node->content[0], "pwd") == 0)
		ft_pwd(ft_mtx_strlen(node->content), node->content);
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
		ft_exit(ft_mtx_strlen(node->content), node->content);
	else
		handle_fork_and_execute(node, *envp);
	i = 0;
	while ((*envp)[i] && ft_strncmp((*envp)[i], "_=", 2) != 0)
		i++;
	free((*envp)[i]);
	(*envp)[i] = tmp;
}

// void ft_expand_error_code(t_node *node)
// {
//     unsigned short i;
//     char *expanded_content;
//     size_t len;
//     if (!node)
//         return ;
//     i = 0;
//     while (node->content[i])
//     {
//         if (ft_strncmp(node->content[i], "$?", 2) == 0) // Check if the string starts with "$?"
//         {
//             // Calculate the new length: length of the error code + remaining part of the string
//             len = ft_strlen(node->content[i]) - 2 + ft_strlen(ft_itoa(g_errno));
//             // Allocate memory for the expanded string
//             expanded_content = malloc(len + 1);
//             if (!expanded_content)
//                 return ; // Handle allocation failure (optional)
//             // Copy the error code into the new string
//             strcpy(expanded_content, ft_itoa(g_errno));
//             // Append the rest of the string (after "$?")
//             strcat(expanded_content, node->content[i] + 2);
//             // Free the old content and assign the new expanded content
//             free(node->content[i]);
//             node->content[i] = expanded_content;
//         }
//         i++;
//     }
// }
// void ft_expand_error_code(t_node *node)
// {
//     unsigned short i;
//     char *found_pos;
//     char *expanded_content;
//     size_t len, original_len;
//     if (!node)
//         return ;
//     i = 0;
//     while (node->content[i])
//     {
//         // Search for "$?" anywhere in the string
//         found_pos = strstr(node->content[i], "$?");
//         // If we find "$?", replace it
//         while (found_pos)
//         {
//             original_len = ft_strlen(node->content[i]);
//             len = ft_strlen(ft_itoa(g_errno)) + (original_len - (found_pos - node->content[i]) - 2);
//             // Allocate memory for the new string
//             expanded_content = malloc(len + 1);
//             if (!expanded_content)
//                 return ; // Handle allocation failure (optional)
//             // Copy the part before "$?"
//             strncpy(expanded_content, node->content[i], found_pos - node->content[i]);
//             // Append the error code (g_errno)
//             strcat(expanded_content, ft_itoa(g_errno));
//             // Append the part after "$?"
//             strcat(expanded_content, found_pos + 2);
//             // Free old content and assign new expanded content
//             free(node->content[i]);
//             node->content[i] = expanded_content;
//             // Search for next occurrence of "$?"
//             found_pos = strstr(node->content[i], "$?");
//         }
//         i++;
//     }
// }
