/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_pointer.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 00:27:06 by anikoyan          #+#    #+#             */
/*   Updated: 2024/04/09 19:02:33 by anikoyan         ###   ########.fr       */
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

static char	*ft_convert_pointer(size_t u_nbr)
{
	char	*hex;
	char	*ptr;

	hex = ft_uitoa_base(u_nbr, 16);
	if (!hex)
		return (NULL);
	ptr = hex;
	ptr = ft_strjoin("0x", hex);
	free(hex);
	if (!ptr)
		return (NULL);
	return (ptr);
}

int	ft_print_pointer(size_t u_nbr, t_flags *flags)
{
	char	*hex;
	int		result;
	int		padding_width;

	result = 0;
	hex = ft_convert_pointer(u_nbr);
	if (!hex)
		return (-1);
	padding_width = ft_padding_width(hex, flags);
	if (ft_print_width(&padding_width, flags, &result) == -1
		|| ft_printf("%s", hex) == -1
		|| ft_print_minus(&padding_width, flags, &result) == -1)
	{
		free(hex);
		return (-1);
	}
	result += (int)ft_strlen(hex);
	free(hex);
	return (result);
}
