/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 19:05:04 by lvarnach          #+#    #+#             */
/*   Updated: 2026/01/26 19:05:09 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#include "../../libs/libft/libft.h"

// If is_custom_err is true, write 'err' directly to stderr by write.
// If the write operation fails, print an error reasons using perror.
// Otherwise, print the 'err' description with perror.
void	print_error(const char *err, bool is_custom_err)
{
	if (is_custom_err == true)
	{
		if (write(2, err, ft_strlen(err)) == -1)
			perror("minishell: write");
		return ;
	}
	perror(err);
}
