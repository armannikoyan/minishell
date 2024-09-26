/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isoperator.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 16:26:18 by anikoyan          #+#    #+#             */
/*   Updated: 2024/09/26 16:35:27 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	ft_isoperator(char *c)
{
	if (!ft_strncmp(c, "||", 2) || !ft_strncmp(c, "|", 1)
		|| !ft_strncmp(c, "&&", 2) || !ft_strncmp(c, ">>", 2)
		|| !ft_strncmp(c, "<<", 2) || !ft_strncmp(c, "<", 1)
		|| !ft_strncmp(c, ">", 1))
		return (true);
	return (false);
}
