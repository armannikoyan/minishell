/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 04:28:05 by anikoyan          #+#    #+#             */
/*   Updated: 2024/04/09 18:47:50 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_putstr(char *str)
{
	int	len;

	len = (int)ft_strlen(str);
	if (write(1, str, len) == -1)
		return (-1);
	return (len);
}
