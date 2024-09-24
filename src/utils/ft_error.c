/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 17:15:07 by anikoyan          #+#    #+#             */
/*   Updated: 2024/09/23 17:37:10 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_errno;

void	ft_error(char *errmsg, unsigned short errno)
{
	ft_printf("minishell: %s%s%s\n", RED, errmsg, WHITE);
	g_errno = errno;
}
