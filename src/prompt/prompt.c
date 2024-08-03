/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasargsy <dasargsy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 20:41:53 by dasargsy          #+#    #+#             */
/*   Updated: 2024/08/03 20:42:40 by dasargsy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static	void	parenthesis_handling(char	*line, t_list *list)
{
		
}

static	char	*quotes_handling(char *line)
{
	
}

void	prompt(char **envp)
{
	t_list 	*list;
	
	list = malloc(sizeof(t_list ));
	while (1)
	{
		printf("%s Minishell:  %s", GREEN, RESET_COLOR);
		if (check_for_parenthesis(list->content) == 1)
		list->content = readline("");
	}
}

