/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 19:33:11 by anikoyan          #+#    #+#             */
/*   Updated: 2024/12/22 23:18:32 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_errno;

void	ft_error(char *str, int m_errno)
{
	char	*tmp;

	if (str)
	{
		tmp = ft_strjoin("\001\x1b[31mminishell:\033[0m\002 ", str);
		if (!tmp)
			exit(EXIT_FAILURE);
		free(str);
		str = ft_strjoin(tmp, "\n\n");
		free(tmp);
		if (!str)
			exit(EXIT_FAILURE);
		write(2, str, ft_strlen(str));
		free(str);
	}
	g_errno = m_errno;
}
