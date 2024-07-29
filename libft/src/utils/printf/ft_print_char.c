/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_char.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 15:51:55 by anikoyan          #+#    #+#             */
/*   Updated: 2024/04/09 19:00:36 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../libft.h"

static int	ft_print_width(t_flags *flags, int *result)
{
	int	padding_width;

	padding_width = flags->width - 1;
	if (flags->minus == 0 && padding_width > 0)
	{
		*result += padding_width;
		if (ft_putchar_n(' ', &padding_width) == -1)
			return (-1);
	}
	return (1);
}

static int	ft_print_minus(t_flags *flags, int *result)
{
	int	padding_width;

	padding_width = flags->width - 1;
	if (flags->minus == 1 && padding_width > 0)
	{
		*result += padding_width;
		if (ft_putchar_n(' ', &padding_width) == -1)
			return (-1);
	}
	return (1);
}

int	ft_print_char(char c, t_flags *flags)
{
	int	result;

	result = 0;
	if (ft_print_width(flags, &result) == -1
		|| ft_putchar(c) == -1 || ft_print_minus(flags, &result) == -1)
		return (-1);
	result++;
	return (result);
}
