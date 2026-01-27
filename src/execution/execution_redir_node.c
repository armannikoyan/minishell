/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_redir_node.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 02:03:11 by lvarnach          #+#    #+#             */
/*   Updated: 2026/01/28 02:11:37 by lvarnach         ###   ########.fr       */
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
#include "hash_table.h"
#include "tokenization.h"
#include "utils.h"
#include "../../libs/libft/libft.h"

static int	setup_heredoc(t_ast_node *node, t_hash_table *ht,
	int *saved_fd, int errnum)
{
	char		*line;
	char		*tmp_name;
	char		quote_char;
	int			fd;
	size_t		i;

	*saved_fd = dup(STDIN_FILENO);
	if (*saved_fd == -1)
		return (print_error("minishell: dup", false), 1);
	fd = open(HEREDOC_TMP_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		close(*saved_fd);
		return (print_error("minishell: open", false), 1);
	}
	quote_char = 0;
	i = -1;
	while (node->u_data.redir.filename[++i])
	{
		set_quote_char(node->u_data.redir.filename[i], &quote_char);
		if (quote_char)
			break ;
	}
	tmp_name = node->u_data.redir.filename;
	node->u_data.redir.filename = remove_quotes(tmp_name);
	free(tmp_name);
	if (!node->u_data.redir.filename)
	{
		(close(fd), close(*saved_fd));
		return (print_error("minishell: malloc", false), 1);
	}
	while (true)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, node->u_data.redir.filename) == 0)
		{
			print_error("minishell: warning: here-document at line 1 delimited by end-of-file (wanted `", true);
			print_error(node->u_data.redir.filename, true);
			print_error("\'\n", true);
			free(line);
			break ;
		}
		if (!quote_char)
		{
			tmp_name = line;
			line = expand_dollar_sign(tmp_name, ht, errnum);
			free(tmp_name);
		}
		if (write(fd, line, ft_strlen(line)) == -1 || write(fd, "\n", 1) == -1)
		{
			(free(line), close(fd), close(*saved_fd));
			return (print_error("minishell: write", false), 1);
		}
		free(line);
	}
	close(fd);
	fd = open(HEREDOC_TMP_FILE, O_RDONLY, 0644);
	if (fd == -1)
	{
		close(*saved_fd);
		return (print_error("minishell: open", false), 1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		(close(fd), close(*saved_fd));
		return (print_error("minishell: dup2", false), 1);
	}
	close(fd);
	return (0);
}

static int	setup_file_redir(t_ast_node *node, int open_flags,
	int std_fd, int *saved_fd)
{
	char	*tmp;

	*saved_fd = dup(std_fd);
	if (*saved_fd == -1)
		return (print_error("minishell: dup", false), 1);
	tmp = node->u_data.redir.filename;
	node->u_data.redir.filename = remove_quotes(tmp);
	free(tmp);
	if (!node->u_data.redir.filename)
	{
		close(*saved_fd);
		return (print_error("minishell: malloc", false), 1);
	}
	node->u_data.redir.fd = open(node->u_data.redir.filename, open_flags, 0644);
	if (node->u_data.redir.fd == -1)
	{
		print_error("minishell: open", false);
		close(*saved_fd);
		return (1);
	}
	if (dup2(node->u_data.redir.fd, std_fd) == -1)
	{
		print_error("minishell: dup2", false);
		close(node->u_data.redir.fd);
		close(*saved_fd);
		return (1);
	}
	close(node->u_data.redir.fd);
	return (0);
}

int	setup_redirection(t_ast_node *node, t_hash_table *ht, int *saved_fd,
	int *target_fd, int errnum)
{
	if (node->type == REDIRECT_IN_NODE)
	{
		*target_fd = STDIN_FILENO;
		return (setup_file_redir(node, O_RDONLY, STDIN_FILENO, saved_fd));
	}
	if (node->type == REDIRECT_OUT_NODE)
	{
		*target_fd = STDOUT_FILENO;
		return (setup_file_redir(node, O_WRONLY | O_CREAT | O_TRUNC,
				STDOUT_FILENO, saved_fd));
	}
	if (node->type == REDIRECT_APPEND_NODE)
	{
		*target_fd = STDOUT_FILENO;
		return (setup_file_redir(node, O_WRONLY | O_CREAT | O_APPEND,
				STDOUT_FILENO, saved_fd));
	}
	if (node->type == HEREDOC_NODE)
	{
		*target_fd = STDIN_FILENO;
		return (setup_heredoc(node, ht, saved_fd, errnum));
	}
	return (1);
}

int	cleanup_redirection(t_ast_node *node, int saved_fd, int target_fd)
{
	if (node->type == HEREDOC_NODE)
		unlink(HEREDOC_TMP_FILE);
	if (dup2(saved_fd, target_fd) == -1)
	{
		print_error("minishell: dup2", false);
		close(saved_fd);
		return (1);
	}
	close(saved_fd);
	return (0);
}

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

int	scan_and_process_heredocs(t_ast_node *node, t_hash_table *ht, int *counter)
{
	char	filename[PATH_MAX];
	int		fd;
	char	*line;
	char	*limiter;

	if (!node)
		return (0);
	if (node->type == HEREDOC_NODE)
	{
		generate_unique_filename(filename, (*counter)++);
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
			return (print_error("minishell: open heredoc", false), 1);
		limiter = remove_quotes(node->u_data.redir.filename);
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
		close(fd);
		free(limiter);
		free(node->u_data.redir.filename);
		node->u_data.redir.filename = ft_strdup(filename);
		node->type = REDIRECT_IN_NODE;
	}
	if (node->type == PIPE_NODE || node->abstract_type == BIN_NODE)
	{
		if (scan_and_process_heredocs(node->u_data.binary.left, ht, counter))
			return (1);
		if (scan_and_process_heredocs(node->u_data.binary.right, ht, counter))
			return (1);
	}
	else if (node->abstract_type == REDIR_NODE)
	{
		if (scan_and_process_heredocs(node->u_data.redir.child, ht, counter))
			return (1);
	}
	return (0);
}

/*
 ** CLEANUP HEREDOCS
 ** loops from 0 to count-1, reconstructing the filename and deleting it.
 */
void	cleanup_heredoc_files(int count)
{
	char	filename[PATH_MAX];
	char	*num_str;
	int		i;

	i = 0;
	while (i < count)
	{
		ft_strlcpy(filename, "/tmp/.heredoc_", 50);
		num_str = ft_itoa(i);
		if (num_str)
		{
			ft_strlcat(filename, num_str, PATH_MAX);
			free(num_str);
			unlink(filename);
		}
		i++;
	}
}
