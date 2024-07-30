/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasargsy <dasargsy@student.42yerevan.am    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 00:11:06 by dasargsy          #+#    #+#             */
/*   Updated: 2024/07/30 04:42:35 by dasargsy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	prompt(char **envp)
{
	char	*line;


	line = NULL;
	(void)envp;
	while (1)
	{
		printf("%sMinishell:  %s", GREEN, RESET_COLOR);
		line = readline("");
		tokenization(line);
	}
}