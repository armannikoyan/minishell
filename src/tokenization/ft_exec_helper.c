/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 23:31:35 by gsimonia          #+#    #+#             */
/*   Updated: 2025/01/15 23:32:59 by gsimonia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_exec_with_level(t_node *node,
		char ***envp, unsigned short *current_level)
{
	if (!node)
		return ;
	if (node->type == 'O')
		ft_exec_operator(node, envp, current_level);
	else
		ft_exec_command(node, envp, current_level);
}
