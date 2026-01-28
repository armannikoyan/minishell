/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_uitoa_base.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 17:28:47 by lvarnach          #+#    #+#             */
/*   Updated: 2024/04/09 18:48:27 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int	ft_itoa_base_strlen(size_t u_nbr, int base)
{
	int	len;

	len = 0;
	while (u_nbr)
	{
		len++;
		u_nbr /= base;
	}
	return (len + 1);
}

static char	ft_match_char(size_t u_nbr)
{
	if (u_nbr >= 10 && u_nbr <= 15)
		return ('a' + u_nbr - 10);
	else
		return ('0' + u_nbr);
}

char	*ft_uitoa_base(size_t u_nbr, int base)
{
	char	*str;
	char	*head;

	if (base < 2 || base > 16)
		return (NULL);
	if (!u_nbr)
		return (ft_strdup("0"));
	str = malloc(sizeof(char) * ft_itoa_base_strlen(u_nbr, base));
	if (!str)
		return (NULL);
	head = str;
	while (u_nbr)
	{
		*str++ = ft_match_char(u_nbr % base);
		u_nbr /= base;
	}
	*str = '\0';
	ft_strrev(head);
	return (head);
}
