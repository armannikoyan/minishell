/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasargsy <dasargsy@student.42yerevan.am    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 23:12:04 by dasargsy          #+#    #+#             */
/*   Updated: 2024/07/29 23:57:11 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void    quote_error(char *line)
{
    write(2, QUOTE_ERROR, 16);
    free(line);
    exit (1);    
}

int	is_closed(char *line, char limiter)
{
	int	i;

	i = 0;
	while (line[i] && line[i] != limiter)
		i++;
	if (line[i] == 0)
		return (0);
	return (1);
}
