/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_report_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 23:20:33 by anikoyan          #+#    #+#             */
/*   Updated: 2025/01/12 07:40:46 by gsimonia         ###   ########.fr       */
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

bool	ft_report_error_arg(const char *prefix,
		const char *content, int error_code, const char *given)
{
	char	*err_msg;
	char	*given_msg;

	given_msg = ft_strjoin(prefix, given);
	err_msg = ft_strjoin(given_msg, content);
	free(given_msg);
	ft_error(err_msg, error_code);
	return (true);
}
