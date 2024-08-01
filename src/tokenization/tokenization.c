/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasargsy <dasargsy@student.42yerevan.am    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 21:59:15 by dasargsy          #+#    #+#             */
/*   Updated: 2024/07/31 03:09:39 by dasargsy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
// static	char	**logic(char *line)
// {
// 	char	**tokens;

	
// }

char	**tokenization(char *line)
{
	(void)line;
	if (valid_tokens(line))
		printf("OK");
	else
		printf("KO");		
	return (NULL);
}