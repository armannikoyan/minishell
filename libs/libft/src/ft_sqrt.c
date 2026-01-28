/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sqrt.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:56:15 by lvarnach          #+#    #+#             */
/*   Updated: 2025/03/17 17:31:11 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static float	ft_fraction(int number, float root)
{
	int		i;
	float	increment;

	i = 0;
	increment = 0.1;
	while (i < 5)
	{
		while (root * root <= number)
			root += increment;
		root = root - increment;
		increment /= 10;
		++i;
	}
	return (root);
}

float	ft_sqrt(int number)
{
	int		start;
	int		mid;
	int		end;
	float	root;

	start = 0;
	end = number;
	root = .0;
	while (start <= end)
	{
		mid = (start + end) / 2;
		if (mid * mid == number)
			return (ft_fraction(number, mid));
		else if (mid * mid < number)
		{
			root = start;
			start = mid + 1;
		}
		else
			end = mid - 1;
	}
	return (ft_fraction(number, root));
}
