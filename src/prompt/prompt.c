/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasargsy <dasargsy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 20:41:53 by dasargsy          #+#    #+#             */
/*   Updated: 2024/08/04 17:35:27 by dasargsy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// static	void	parenthesis_handling(char	*line, t_list *list)
// {
		
// }

static	char	*quotes_handling(char *line, int limiter)
{
	
}

void	prompt(char **envp)
{
	t_list 	*list;
	
	list = malloc(sizeof(t_list ));
	while (1)
	{
		printf("%s Minishell:  %s", GREEN, RESET_COLOR);
		list->content = readline("");
		if (check_for_quotes(list->content, 34))
			list->content = quotes_handling(list->content, 34);
		if (check_for_quotes(list->content, 39))
			list->content = quotes_handling(list->content, 39);
	}
}

