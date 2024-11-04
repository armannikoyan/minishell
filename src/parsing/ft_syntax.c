/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_syntax.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 19:38:41 by anikoyan          #+#    #+#             */
/*   Updated: 2024/10/23 18:47:52 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	ft_has_syntax_error(t_list **lst)
{
	t_list	*tmp;

	tmp = *lst;
	while (tmp)
	{
		ft_printf("----------\n%s\n----------\n", tmp->content);
		tmp = tmp->next;
	}
	tmp = *lst;
	while (tmp && tmp->next)
	{
		if (ft_isoperator(tmp->content) || !ft_strncmp(tmp->content, "&", 1))
		{
			if (ft_isoperator(tmp->next->content) || !ft_strncmp(tmp->next->content, "&", 1))
				ft_printf("Error near %s\n", tmp->next->content);
		}
		tmp = tmp->next;
	}
	return (0);
}
