/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 20:11:19 by anikoyan          #+#    #+#             */
/*   Updated: 2025/12/06 01:38:17 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#include "../../libs/libft/libft.h"

void	print_error(const char *err)
{
	if (write(2, err, ft_strlen(err)) == -1)
		perror("write");
}
