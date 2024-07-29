/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_int.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 21:44:29 by anikoyan          #+#    #+#             */
/*   Updated: 2024/04/09 19:01:24 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../libft.h"

int	ft_print_int(int nbr, t_flags *flags)
{
	char	*number;
	int		result;
	int		len;
	int		padding_width;

	number = ft_itoa(nbr);
	if (!number)
		return (-1);
	result = 0;
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
