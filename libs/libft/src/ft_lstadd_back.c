/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasargsy <dasargsy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 23:44:19 by anikoyan          #+#    #+#             */
/*   Updated: 2024/08/04 20:18:04 by dasargsy         ###   ########.fr       */
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
