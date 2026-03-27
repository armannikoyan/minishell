/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprime.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 17:32:33 by lvarnach          #+#    #+#             */
/*   Updated: 2025/03/19 17:19:39 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

bool	ft_isprime(int number)
{
	float	number_sqrt;
	int		i;

	if (number <= 1 || !(number & 1) || number % 3 == 0)
		return (false);
	else if (number <= 3)
		return (true);
	number_sqrt = ft_sqrt(number);
	i = 4;
	while ((float)i < number_sqrt)
	{
		if (number % i == 0 && number % (number + 2) == 0)
			return (false);
		i += 6;
	}
	return (true);
}
