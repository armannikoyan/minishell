/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 23:46:40 by lvarnach          #+#    #+#             */
/*   Updated: 2024/04/09 18:28:22 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char		*dst_ptr;
	const unsigned char	*src_ptr;

	dst_ptr = (unsigned char *)dst;
	src_ptr = (const unsigned char *)src;
	if (dst_ptr == src_ptr || len == 0)
		return (dst);
	if (dst < src)
		return (ft_memcpy(dst, src, len));
	while (len--)
		dst_ptr[len] = src_ptr[len];
	return (dst);
}
