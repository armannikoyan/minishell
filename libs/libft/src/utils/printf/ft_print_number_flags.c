/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_number_flags.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 11:03:51 by anikoyan          #+#    #+#             */
/*   Updated: 2024/04/09 19:02:44 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../libft.h"

static char	*ft_removeminus(char *number, int *result, int *len)
{
	char	*ptr;

	if (number[0] == '-')
	{
		*result += 1;
		if (ft_putchar('-') == -1)
		{
			free(number);
			return (NULL);
		}
		ptr = ft_strdup(number + 1);
		free(number);
		if (!ptr)
			return (NULL);
		number = ptr;
		(*len)--;
	}
	return (number);
}

static int	ft_process_hash(char *number, int *result, t_flags *flags)
{
	if (flags->hash == 1 && number[0] != '0' && (int)ft_strlen(number) > 0)
	{
		if (flags->upper)
		{
			if (write(1, "0X", 2) == -1)
			{
				free(number);
				return (-1);
			}
		}
		else
		{
			if (write(1, "0x", 2) == -1)
			{
				free(number);
				return (-1);
			}
		}
		*result += 2;
	}
	return (1);
}

static char	*ft_process_minus_space(char *number, int *padding_width
	, int *result, t_flags *flags)
{
	if (flags->minus == 0 && *padding_width > 0)
	{
		*result += *padding_width;
		if (ft_putchar_n(' ', padding_width) == -1)
		{
			free(number);
			return (NULL);
		}
	}
	if (ft_process_hash(number, result, flags) == -1)
		return (NULL);
	if (flags->space == 1 && number[0] != '-' && flags->plus == 0
		&& (flags->minus == -1 || (flags->minus == 0
				&& *padding_width <= 0) || flags->minus == 1))
	{
		*result += 1;
		if (ft_putchar(' ') == -1)
		{
			free(number);
			return (NULL);
		}
		(*padding_width)--;
	}
	return (number);
}

static char	*ft_process_zero_precision(char *number, int *padding_width
	, int *result, t_flags *flags)
{
	int	len;

	len = (int)ft_strlen(number);
	number = ft_removeminus(number, result, &len);
	if (flags->zero == 1 && *padding_width > 0)
	{
		*result += *padding_width;
		if (ft_putchar_n('0', padding_width) == -1)
		{
			free(number);
			return (NULL);
		}
	}
	if (flags->precision - len > 0)
	{
		flags->precision -= len;
		*result += flags->precision;
		if (ft_putchar_n('0', &flags->precision) == -1)
		{
			free(number);
			return (NULL);
		}
	}
	return (number);
}

char	*ft_process_flags(char *number, int *padding_width
		, int *result, t_flags *flags)
{
	if (flags->precision == 0 && number[0] == '0')
	{
		free(number);
		number = ft_strdup("");
		if (!number)
			return (NULL);
	}
	*padding_width = ft_padding_width(number, flags);
	number = ft_process_minus_space(number, padding_width, result, flags);
	if (!number)
		return (NULL);
	if (flags->plus == 1 && number[0] != '-')
	{
		*result += 1;
		if (ft_putchar('+') == -1)
		{
			free(number);
			return (NULL);
		}
	}
	number = ft_process_zero_precision(number, padding_width, result, flags);
	return (number);
}
