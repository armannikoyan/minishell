/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 13:23:47 by lvarnach          #+#    #+#             */
/*   Updated: 2024/07/23 14:18:45 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static void	ft_sign(int *sign, const char **str)
{
	while (**str == '+' || **str == '-')
	{
		if (**str == '-')
			*sign = -(*sign);
		(*str)++;
	}
}

double	ft_atof(const char *str)
{
	double	fraction;
	int		sign;
	double	pow;

	if (!str)
		return (0);
	fraction = 0;
	sign = 1;
	pow = 1;
	while (ft_isspace(*str))
		str++;
	ft_sign(&sign, &str);
	while (ft_isdigit(*str))
		fraction = (fraction * 10) + (*str++ - '0');
	if (*str == '.')
		str++;
	while (ft_isdigit(*str))
	{
		pow /= 10;
		fraction += (*str++ - '0') * pow;
	}
	return (fraction * sign);
}
