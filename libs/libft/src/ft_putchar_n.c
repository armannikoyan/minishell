/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_n.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 04:31:41 by lvarnach          #+#    #+#             */
/*   Updated: 2024/04/09 18:47:32 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

int	ft_putchar_n(char c, int *count)
{
	char	*str;
	int		i;

	str = (char *)malloc(sizeof(char) * (*count + 1));
	if (!str)
		return (-1);
	i = 0;
	while (i < *count)
		str[i++] = c;
	str[i] = '\0';
	if (write(1, str, *count) == -1)
		*count = -1;
	free(str);
	return (*count);
}
