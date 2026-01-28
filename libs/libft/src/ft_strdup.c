/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 23:48:25 by lvarnach          #+#    #+#             */
/*   Updated: 2024/04/09 18:30:17 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strdup(const char *s1)
{
	char	*dest;
	int		s1_len;

	s1_len = ft_strlen(s1);
	dest = (char *)malloc(sizeof(char) * (s1_len + 1));
	if (!dest)
		return (NULL);
	return ((char *)ft_memcpy(dest, s1, s1_len + 1));
}
