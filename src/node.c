/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 21:28:23 by anikoyan          #+#    #+#             */
/*   Updated: 2024/07/29 21:47:03 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/node.h"

t_node	*node_ctor()
{
	t_node	*node;

	node = (t_node *)malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->left = NULL;
	node->data = 0;
	node->right = NULL;
	return (node);
}

void	*node_dtor(t_node *node)
{
	if (node)
		free(node);
	return (NULL);
}
