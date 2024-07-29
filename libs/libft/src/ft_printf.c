/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 18:52:25 by anikoyan          #+#    #+#             */
/*   Updated: 2024/04/09 18:39:48 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int	ft_print_type(char c, t_flags *flags, va_list *listp)
{
	if (c == 'c')
		return (ft_print_char(va_arg(*listp, int), flags));
	else if (c == 's')
		return (ft_print_string(va_arg(*listp, char *), flags));
	else if (c == 'd' || c == 'i')
		return (ft_print_int(va_arg(*listp, int), flags));
	else if (c == 'u')
		return (ft_print_unsigned(va_arg(*listp, unsigned int), flags));
	else if (c == 'x' || c == 'X')
	{
		if (c == 'X')
			flags->upper = 1;
		return (ft_print_hex(va_arg(*listp, unsigned int), flags));
	}
	else if (c == 'p')
		return (ft_print_pointer(va_arg(*listp, size_t), flags));
	else if (c == '%')
		return (ft_print_char('%', flags));
	return (0);
}

int	ft_printf(const char *fmt, ...)
{
	int		result;
	int		tmp;
	va_list	listp;
	t_flags	flags;

	va_start(listp, fmt);
	result = 0;
	while (*fmt && result != -1)
	{
		if (*fmt == '%')
		{
			flags = ft_printf_parse_flags(++fmt);
			while (ft_strchr("0.-# +", *fmt) || ft_isdigit(*fmt))
				fmt++;
			tmp = ft_print_type(*fmt, &flags, &listp);
		}
		else
			tmp = ft_putchar(*fmt);
		result += tmp;
		if (tmp == -1)
			result = -1;
		fmt++;
	}
	va_end(listp);
	return (result);
}
