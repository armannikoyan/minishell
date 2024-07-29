/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_string.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 16:08:48 by anikoyan          #+#    #+#             */
/*   Updated: 2024/04/09 19:00:55 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../libft.h"

static int	ft_print_width(int *padding_width, t_flags *flags, int *result)
{
	if (flags->minus == 0 && *padding_width > 0)
	{
		*result += *padding_width;
		if (ft_putchar_n(' ', padding_width) == -1)
			return (-1);
	}
	return (1);
}

static int	ft_print_minus(int *padding_width, t_flags *flags, int *result)
{
	if (flags->minus == 1 && *padding_width > 0)
	{
		*result += *padding_width;
		if (ft_putchar_n(' ', padding_width) == -1)
			return (-1);
	}
	return (1);
}

int	ft_print_string(char *str, t_flags *flags)
{
	int	result;
	int	len;
	int	padding_width;

	if (!str)
		str = "(null)";
	result = 0;
	len = ft_strlen(str);
	if (flags->precision >= 0 && flags->precision < len)
		len = flags->precision;
	padding_width = flags->width - len;
	if (ft_print_width(&padding_width, flags, &result) == -1
		|| write(1, str, len) == -1
		|| ft_print_minus(&padding_width, flags, &result) == -1)
		return (-1);
	result += len;
	return (result);
}
