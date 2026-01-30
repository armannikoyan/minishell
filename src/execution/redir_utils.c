/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 17:26:37 by lvarnach          #+#    #+#             */
/*   Updated: 2026/01/31 02:30:59 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>
#include <sys/fcntl.h>

#include "ast.h"
#include "execution.h"
#include "expansion.h"
#include "utils.h"
#include "../libs/libft/libft.h"

int	apply_dup2(int fd, int target_fd, int *saved_fd)
{
	if (dup2(fd, target_fd) == -1)
	{
		print_error("minishell: dup2", false);
		close(fd);
		close(*saved_fd);
		return (1);
	}
	return (0);
}

int	prepare_filename(t_ast_node *node, int *saved_fd)
{
	char	*tmp;

	tmp = node->u_data.redir.filename;
	node->u_data.redir.filename = remove_quotes(tmp);
	free(tmp);
	if (!node->u_data.redir.filename)
	{
		close(*saved_fd);
		return (print_error("minishell: malloc", false), 1);
	}
	return (0);
}

int	setup_file_redir(t_ast_node *node, int flags, int std_fd, int *save)
{
	int	fd;

	*save = dup(std_fd);
	if (*save == -1)
		return (print_error("minishell: dup", false), 1);
	if (prepare_filename(node, save))
		return (1);
	fd = open(node->u_data.redir.filename, flags, 0644);
	if (fd == -1)
	{
		print_error("minishell: open", false);
		close(*save);
		return (1);
	}
	if (apply_dup2(fd, std_fd, save))
		return (1);
	close(fd);
	return (0);
}

int	handle_heredoc_line(char *line, t_doc_ctx *ctx)
{
	char	*expanded;

	if (ft_strcmp(line, ctx->limiter) == 0)
	{
		free(line);
		return (1);
	}
	if (!ctx->quoted)
	{
		expanded = expand_dollar_sign(line, ctx->ht, ctx->errnum);
		free(line);
		line = expanded;
	}
	if (write(ctx->fd, line, ft_strlen(line)) == -1
		|| write(ctx->fd, "\n", 1) == -1)
	{
		free(line);
		return (-1);
	}
	free(line);
	return (0);
}

int	process_heredoc_loop(t_doc_ctx *ctx)
{
	char	*line;
	int		status;

	while (true)
	{
		line = readline("> ");
		if (!line)
		{
			print_error("minishell: warning: here-document delimited by EOF",
				true);
			break ;
		}
		status = handle_heredoc_line(line, ctx);
		if (status == 1)
			break ;
		if (status == -1)
			return (1);
	}
	return (0);
}
