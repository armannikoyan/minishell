/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_parse_flags.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 21:01:48 by anikoyan          #+#    #+#             */
/*   Updated: 2024/04/09 19:02:55 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../libft.h"

static void	ft_init_flags(t_flags *flags)
{
	flags->zero = 0;
	flags->minus = -1;
	flags->hash = 0;
	flags->space = 0;
	flags->plus = 0;
	flags->width = 0;
	flags->precision = -1;
	flags->upper = 0;
}

static void	ft_parse_width(const char *fmt, t_flags *flags)
{
	if (ft_isdigit(*fmt))
	{
		if (!flags->zero && flags->minus == -1)
			flags->minus = 0;
		flags->width = ft_atoi(fmt);
	}
}

static void	ft_parse_precision(const char *fmt, t_flags *flags)
{
	if (*fmt == '.')
	{
		fmt++;
		if (ft_isdigit(*fmt))
			flags->precision = ft_atoi(fmt);
		else
			flags->precision = 0;
	}
}

t_flags	ft_printf_parse_flags(const char *fmt)
{
	t_flags	flags;

	ft_init_flags(&flags);
	while (*fmt && ft_strchr("0-# +", *fmt))
	{
		if (*fmt == '0')
			flags.zero = 1;
		else if (*fmt == '-')
			flags.minus = 1;
		else if (*fmt == '#')
			flags.hash = 1;
		else if (*fmt == ' ')
			flags.space = 1;
		else if (*fmt == '+')
			flags.plus = 1;
		fmt++;
	}
	ft_parse_width(fmt, &flags);
	while (ft_isdigit(*fmt))
		fmt++;
	ft_parse_precision(fmt, &flags);
	return (flags);
}
