/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 01:45:25 by lvarnach          #+#    #+#             */
/*   Updated: 2026/01/28 01:52:26 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "ast.h"
#include "hash_table.h"
#include "utils.h"
#include "execution.h"
#include "term_settings.h"
#include "../../libs/libft/libft.h"

static void	del_frame(void *content)
{
	free(content);
}

int	execute_subshell(t_ast_node *node, t_hash_table *ht, int errnum)
{
	pid_t		pid;
	int			status;

	pid = fork();
	if (pid == -1)
	{
		print_error("minishell: fork", false);
		return (1);
	}
	if (pid)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		status = handle_child_exit(pid);
		psig_set();
		return (status);
	}
	else
		exit(execute(node->u_data.subshell.root, ht, errnum));
}

static int	push_new_frame(t_list **stack, t_ast_node *node)
{
	t_exec_frame		*frame;
	t_list				*new_node;

	if (!node)
		return (0);
	frame = malloc(sizeof(t_exec_frame));
	if (!frame)
		return (1);
	frame->node = node;
	frame->state = 0;
	frame->saved_fd = -1;
	frame->target_fd = -1;
	new_node = ft_lstnew(frame);
	if (!new_node)
	{
		free(frame);
		return (1);
	}
	ft_lstadd_front(stack, new_node);
	return (0);
}

int	execute(t_ast_node *root, t_hash_table *ht, int errnum)
{
	t_list					*stack;
	t_exec_frame			*curr;
	t_list					*temp;
	int						last_status;
	int						perform_right;

	if (!root)
		return (0);
	stack = NULL;
	last_status = errnum;
	if (push_new_frame(&stack, root) != 0)
		return (1);
	while (stack)
	{
		curr = (t_exec_frame *)stack->content;
		if (curr->state == 0)
		{
			if (curr->node->type == PIPE_NODE)
			{
				last_status = execute_pipeline(curr->node, ht, last_status);
				temp = stack;
				stack = stack->next;
				ft_lstdelone(temp, del_frame);
			}
			else if (curr->node->abstract_type == BIN_NODE)
			{
				curr->state = 1;
				push_new_frame(&stack, curr->node->u_data.binary.left);
			}
			else if (curr->node->abstract_type == REDIR_NODE)
			{
				if (setup_redirection(curr->node, ht, &curr->saved_fd,
						&curr->target_fd, last_status) != 0)
				{
					last_status = 1;
					temp = stack;
					stack = stack->next;
					ft_lstdelone(temp, del_frame);
				}
				else
				{
					curr->state = 2;
					push_new_frame(&stack, curr->node->u_data.redir.child);
				}
			}
			else
			{
				if (curr->node->type == SUBSHELL_NODE)
					last_status = execute_subshell(curr->node, ht, last_status);
				else if (curr->node->abstract_type == CMD_NODE)
					last_status = execute_command(curr->node, ht, last_status);
				temp = stack;
				stack = stack->next;
				ft_lstdelone(temp, del_frame);
			}
		}
		else if (curr->state == 1)
		{
			perform_right = 0;
			if (curr->node->type == AND_NODE && last_status == 0)
				perform_right = 1;
			else if (curr->node->type == OR_NODE && last_status != 0)
				perform_right = 1;
			if (perform_right)
			{
				curr->node = curr->node->u_data.binary.right;
				curr->state = 0;
			}
			else
			{
				temp = stack;
				stack = stack->next;
				ft_lstdelone(temp, del_frame);
			}
		}
		else if (curr->state == 2)
		{
			cleanup_redirection(curr->node, curr->saved_fd, curr->target_fd);
			temp = stack;
			stack = stack->next;
			ft_lstdelone(temp, del_frame);
		}
	}
	return (last_status);
}
