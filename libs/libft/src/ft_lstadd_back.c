/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 23:44:19 by lvarnach          #+#    #+#             */
/*   Updated: 2024/04/09 18:26:14 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new_node)
{
	t_list	*i;

	if (!*lst)
		*lst = new_node;
	else
	{
		i = ft_lstlast(*lst);
		i->next = new_node;
	}
}
