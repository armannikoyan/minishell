/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 23:44:03 by lvarnach          #+#    #+#             */
/*   Updated: 2024/04/09 18:26:06 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int	ft_digit_count(int n)
{
	int	count;

	count = 0;
	if (!n)
		count++;
	while (n)
	{
		count++;
		n /= 10;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	char	*number;
	int		str_len;

	if (n == INT_MIN)
		return (ft_strdup("-2147483648\0"));
	str_len = ft_digit_count(n);
	if (n < 0)
		str_len++;
	number = malloc(sizeof(char) * (str_len + 1));
	if (!number)
		return (NULL);
	number[str_len] = '\0';
	if (n == 0)
		number[0] = '0';
	else if (n < 0)
	{
		number[0] = '-';
		n *= -1;
	}
	while (n)
	{
		number[--str_len] = n % 10 + '0';
		n /= 10;
	}
	return (number);
}
