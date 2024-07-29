/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 23:43:09 by anikoyan          #+#    #+#             */
/*   Updated: 2024/04/09 18:25:17 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int	ft_isupper(int c)
{
	return ('A' <= c && 'Z' >= c);
}

static int	ft_islower(int c)
{
	return ('a' <= c && 'z' >= c);
}

int	ft_isalpha(int c)
{
	return (ft_isupper(c) || ft_islower(c));
}
