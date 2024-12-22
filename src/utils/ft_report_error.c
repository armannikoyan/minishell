/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_report_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 23:20:33 by anikoyan          #+#    #+#             */
/*   Updated: 2024/12/22 23:22:47 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	ft_report_error(const char *prefix, const char *content, int error_code)
{
	char	*err_msg;

	err_msg = ft_strjoin(prefix, content);
	ft_error(err_msg, error_code);
	return (true);
}
