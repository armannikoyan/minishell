/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 01:29:13 by lvarnach          #+#    #+#             */
/*   Updated: 2026/02/03 00:50:32 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/errno.h>
#include <sys/wait.h>

#include "error_codes.h"
#include "execution.h"
#include "tokenization.h"
#include "utils.h"

static size_t	get_clean_len(char *str)
{
	size_t	len;
	size_t	i;
	char	quote_state;
	char	prev_state;

	len = 0;
	i = 0;
	quote_state = 0;
	while (str[i])
	{
		prev_state = quote_state;
		set_quote_char(str[i], &quote_state);
		if (prev_state == quote_state)
			len++;
		i++;
	}
	return (len);
}

char	*remove_quotes(char *str)
{
	char	*new_str;
	char	quote_state;
	char	prev_state;
	size_t	i;
	size_t	j;

	if (!str)
		return (NULL);
	new_str = (char *)malloc(sizeof(char) * (get_clean_len(str) + 1));
	if (!new_str)
		return (NULL);
	i = 0;
	j = 0;
	quote_state = 0;
	while (str[i])
	{
		prev_state = quote_state;
		set_quote_char(str[i], &quote_state);
		if (prev_state == quote_state)
			new_str[j++] = str[i];
		i++;
	}
	new_str[j] = '\0';
	return (new_str);
}

int	handle_child_exit(pid_t pid)
{
	int	status;
	int	sig;

	if (waitpid(pid, &status, 0) == -1)
	{
		if (errno != ECHILD)
			print_error("minishell: waitpid", false);
		return (1);
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT || sig == SIGPIPE)
			printf("^C\n");
		else if (sig == SIGQUIT)
			printf("^\\Quit: signum: %d\n", sig);
		return (RESERVED_ERROR_CODES + sig);
	}
	return (1);
}

void	handle_redir_init(t_exec_frame *frame, t_exec_ctx *ctx)
{
	if (setup_redirection(frame->node, &frame->saved_fd, &frame->target_fd))
	{
		*ctx->status = 1;
		pop_frame(ctx->stack);
	}
	else
	{
		frame->state = 2;
		push_new_frame(ctx->stack, frame->node->u_data.redir.child);
	}
}
