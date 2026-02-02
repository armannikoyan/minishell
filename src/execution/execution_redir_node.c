/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_redir_node.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 02:03:11 by lvarnach          #+#    #+#             */
/*   Updated: 2026/02/02 21:38:59 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <stdio.h>
#include <readline/readline.h>
#include <signal.h>
#include <sys/wait.h>

#include "ast.h"
#include "execution.h"
#include "expansion.h"
#include "term_settings.h"
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

// Child process: Writes input and CLEANS UP MEMORY
static void	run_heredoc_child(int fd, char *limiter, t_garbage *g)
{
	signal(SIGINT, SIG_DFL);
	write_heredoc_to_file(fd, limiter);
	close(fd);
	free(limiter);
	clean_all_stacks(g);
	ast_deletion(g->root);
	ht_destroy(g->ht);
	exit(0);
}

static int	process_single_heredoc(t_ast_node *node, int *counter, t_garbage *g)
{
	char	filename[PATH_MAX];
	int		fd;
	char	*limiter;
	pid_t	pid;
	int		status;

	generate_unique_filename(filename, (*counter)++);
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (print_error("minishell: open heredoc", false), 1);
	limiter = remove_quotes(node->u_data.redir.filename);
	pid = fork();
	if (pid == -1)
		return (close(fd), free(limiter),
			print_error("minishell: fork", false), 1);
	if (pid == 0)
		run_heredoc_child(fd, limiter, g);
	signal(SIGINT, SIG_IGN);
	(waitpid(pid, &status, 0), psig_set(), close(fd), free(limiter));
	if (WIFSIGNALED(status))
		return (1);
	free(node->u_data.redir.filename);
	node->u_data.redir.filename = ft_strdup(filename);
	node->type = REDIRECT_IN_NODE;
	return (0);
}

int	scan_and_process_heredocs(t_ast_node *n, t_hash_table *ht,
		t_ast_node *root, int *hc)
{
	int	status;

	if (!n)
		return (0);
	if (n->type == HEREDOC_NODE)
	{
		status = process_single_heredoc(n, hc, &(t_garbage)
			{.stack = NULL, .root = root, .ht = ht, .next = NULL});
		if (status)
			return (1);
	}
	if ((n->type == PIPE_NODE || n->type == AND_NODE || n->type == OR_NODE)
		&& (scan_and_process_heredocs(n->u_data.binary.left, ht, root, hc)
			|| scan_and_process_heredocs(n->u_data.binary.right, ht, root, hc)))
		return (1);
	else if (n->abstract_type == REDIR_NODE)
	{
		if (scan_and_process_heredocs(n->u_data.redir.child, ht, root, hc))
			return (1);
	}
	else if (n->type == SUBSHELL_NODE
		&& scan_and_process_heredocs(n->u_data.subshell.root, ht, root, hc))
		return (1);
	return (0);
}
