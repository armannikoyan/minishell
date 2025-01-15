/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_syntax_node_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 23:27:48 by anikoyan          #+#    #+#             */
/*   Updated: 2025/01/15 23:28:31 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	parse_o_node(t_list **lst, char ***content, char *type_of_node)
{
	t_list			*tmp;
	unsigned short	i;

	*type_of_node = 'O';
	tmp = *lst;
	i = 0;
	(*content)[i++] = ((t_token *)tmp->content)->content;
	tmp = tmp->next;
	if (tmp && (t_token *)tmp->content
		&& ((t_token *)tmp->content)->type == 'F')
	{
		(*content)[i++] = ((t_token *)tmp->content)->content;
		tmp = tmp->next;
	}
	(*content)[i] = NULL;
}
