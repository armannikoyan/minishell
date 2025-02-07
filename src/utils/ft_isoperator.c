/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isoperator.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 16:26:18 by anikoyan          #+#    #+#             */
/*   Updated: 2025/01/15 00:58:25 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

unsigned short	ft_isoperator(char *c)
{
	if (!ft_strncmp(c, "||", 2) || !ft_strncmp(c, "&&", 2)
		|| !ft_strncmp(c, ">>", 2) || !ft_strncmp(c, "<<", 2))
		return (2);
	if (!ft_strncmp(c, "|", 1) || !ft_strncmp(c, ">", 1)
		|| !ft_strncmp(c, "<", 1) || !ft_strncmp(c, "&", 1))
		return (1);
	return (0);
}
