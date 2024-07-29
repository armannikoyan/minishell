/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 21:26:25 by anikoyan          #+#    #+#             */
/*   Updated: 2024/07/29 23:44:15 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NODE_H
# define NODE_H

# include "../libs/libft/libft.h"

typedef struct s_node
{
	struct s_node	*left;
	int				data;
	struct s_node	*right;
}		t_node;

#endif
