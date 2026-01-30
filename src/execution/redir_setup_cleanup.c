/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_setup_cleanup.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 17:26:02 by lvarnach          #+#    #+#             */
/*   Updated: 2026/01/28 17:26:25 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/fcntl.h>
#include <limits.h>

#include "ast.h"
#include "execution.h"
#include "tokenization.h"
#include "utils.h"
#include "../libs/libft/libft.h"

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

void	prepare_limiter(t_ast_node *node, t_doc_ctx *ctx)
{
	char	*tmp;
	size_t	i;

	ctx->quoted = 0;
	i = -1;
	while (node->u_data.redir.filename[++i])
	{
		set_quote_char(node->u_data.redir.filename[i], (char *)&ctx->quoted);
		if (ctx->quoted)
			break ;
	}
	tmp = node->u_data.redir.filename;
	ctx->limiter = remove_quotes(tmp);
	node->u_data.redir.filename = ctx->limiter;
	free(tmp);
}

int	setup_heredoc(t_ast_node *node, t_hash_table *ht, int *s_fd, int e)
{
	t_doc_ctx	ctx;
	int			fd;

	*s_fd = dup(STDIN_FILENO);
	if (*s_fd == -1)
		return (print_error("minishell: dup", false), 1);
	ctx.fd = open(HEREDOC_TMP_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (ctx.fd == -1)
		return (close(*s_fd), print_error("minishell: open", false), 1);
	prepare_limiter(node, &ctx);
	ctx.ht = ht;
	ctx.errnum = e;
	if (!ctx.limiter || process_heredoc_loop(&ctx))
		return (close(ctx.fd), close(*s_fd), 1);
	close(ctx.fd);
	fd = open(HEREDOC_TMP_FILE, O_RDONLY, 0644);
	if (fd == -1)
		return (close(*s_fd), print_error("minishell: open", false), 1);
	if (dup2(fd, STDIN_FILENO) == -1)
		return (close(fd), close(*s_fd), print_error("minishell: dup2", 0), 1);
	return (close(fd), 0);
}

// Dispatcher for execution (Heredocs should already be processed)
int	setup_redirection(t_ast_node *node, t_hash_table *ht,
	int *saved_fd, int *target_fd, t_garbage *g)
{
	int	flags;
	int	std_fd;

	(void)ht;
	(void)g;

	if (node->type == REDIRECT_IN_NODE)
	{
		flags = O_RDONLY;
		std_fd = STDIN_FILENO;
	}
	else if (node->type == REDIRECT_OUT_NODE)
	{
		flags = O_WRONLY | O_CREAT | O_TRUNC;
		std_fd = STDOUT_FILENO;
	}
	else if (node->type == REDIRECT_APPEND_NODE)
	{
		flags = O_WRONLY | O_CREAT | O_APPEND;
		std_fd = STDOUT_FILENO;
	}
	else
		return (0);

	// --- FIX START ---
	// We MUST save which FD we are replacing (0, 1, or 2)
	// so cleanup_redirection knows where to restore the saved_fd to.
	*target_fd = std_fd;
	// --- FIX END ---

	return (setup_file_redir(node, flags, std_fd, saved_fd));
}

void cleanup_redirection(t_ast_node *node, int saved_fd, int target_fd)
{
	(void)node; // might be unused
	if (saved_fd != -1)
	{
		// Restore the original FD (e.g., puts stdout back to terminal)
		dup2(saved_fd, target_fd);
		close(saved_fd);
	}
}
