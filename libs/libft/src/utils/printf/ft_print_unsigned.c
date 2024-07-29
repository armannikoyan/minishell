/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_unsigned.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 19:00:15 by anikoyan          #+#    #+#             */
/*   Updated: 2024/04/09 19:01:12 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../libft.h"

int	ft_print_unsigned(unsigned int u_nbr, t_flags *flags)
{
	char	*number;
	int		result;
	int		len;
	int		padding_width;

	result = 0;
	number = ft_uitoa(u_nbr);
	if (!number)
		return (-1);
	padding_width = ft_padding_width(number, flags);
	number = ft_process_flags(number, &padding_width, &result, flags);
	if (!number)
		return (-1);
	len = ft_putstr(number);
	free(number);
	if (len == -1)
		return (-1);
	result += len;
	if (flags->minus == 1 && padding_width > 0)
	{
		result += padding_width;
		if (ft_putchar_n(' ', &padding_width) == -1)
			return (-1);
	}
	return (result);
}
