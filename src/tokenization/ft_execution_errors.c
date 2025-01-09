/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execution_errors.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 16:22:38 by gsimonia          #+#    #+#             */
/*   Updated: 2025/01/09 16:22:39 by gsimonia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_errno;

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

void	handle_fork_error(void)
{
	perror("fork failed");
	g_errno = 1;
}

void	ft_exec(t_tree *tree, char **envp)
{
	static unsigned short	current_level;

	current_level = 0;
	if (!tree || !tree->root)
		return ;
	ft_exec_with_level(tree->root, envp, &current_level);
}
