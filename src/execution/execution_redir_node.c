/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_redir_node.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 02:03:11 by lvarnach          #+#    #+#             */
/*   Updated: 2026/01/28 17:13:24 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <stdio.h>
#include <readline/readline.h>

#include "ast.h"
#include "execution.h"
#include "expansion.h"
#include "tokenization.h"
#include "utils.h"
#include "../../libs/libft/libft.h"

static void	generate_unique_filename(char *buffer, int index)
{
	char	*num_str;

	ft_strlcpy(buffer, "/tmp/.heredoc_", 50);
	num_str = ft_itoa(index);
	if (num_str)
	{
		ft_strlcat(buffer, num_str, 50);
		free(num_str);
	}
}

static int	write_heredoc_to_file(int fd, char *limiter)
{
	char	*line;

	while (true)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, limiter) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	return (0);
}

static int	process_single_heredoc(t_ast_node *node, int *counter)
{
	char	filename[PATH_MAX];
	int		fd;
	char	*limiter;

	generate_unique_filename(filename, (*counter)++);
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (print_error("minishell: open heredoc", false), 1);
	limiter = remove_quotes(node->u_data.redir.filename);
	write_heredoc_to_file(fd, limiter);
	close(fd);
	free(limiter);
	free(node->u_data.redir.filename);
	node->u_data.redir.filename = ft_strdup(filename);
	node->type = REDIRECT_IN_NODE;
	return (0);
}

int	scan_and_process_heredocs(t_ast_node *node, t_hash_table *ht, int *count)
{
	if (!node)
		return (0);
	if (node->type == HEREDOC_NODE)
	{
		if (process_single_heredoc(node, count))
			return (1);
	}
	if (node->type == PIPE_NODE || node->abstract_type == BIN_NODE)
	{
		if (scan_and_process_heredocs(node->u_data.binary.left, ht, count))
			return (1);
		if (scan_and_process_heredocs(node->u_data.binary.right, ht, count))
			return (1);
	}
	else if (node->abstract_type == REDIR_NODE)
	{
		if (scan_and_process_heredocs(node->u_data.redir.child, ht, count))
			return (1);
	}
	return (0);
}
