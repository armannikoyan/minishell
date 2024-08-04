/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasargsy <dasargsy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 20:41:53 by dasargsy          #+#    #+#             */
/*   Updated: 2024/08/04 20:17:35 by dasargsy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static	int		sz(char *line, char limiter)
{
	int	i;
	int	sz;

	i = 0;
	sz = 0;
	while (line[i])
	{
		if (line[i] == limiter)
			sz++;
		i++;
	}
	return (sz);
}

static	void	parenthesis_handling(char	*line, t_list **head)
{
	char	*tmp;
	int		l_sz;
	int		r_sz;

	tmp = NULL;
	l_sz = sz(line, '(');
	r_sz = 0;
	while (1)
	{
		printf("> ");
		tmp = readline("");
		r_sz += sz(tmp, ')');
		ft_lstadd_back(head, ft_lstnew(tmp));
		free(tmp);
		if (r_sz == l_sz)
			return ;
	}
}

static	char	*quotes_handling(char *line, char limiter)
{
	char	*tmp;

	tmp = NULL;
	while (1)
	{
		printf("> ");
		tmp = readline("");
		if (tmp[ft_strlen(tmp) - 1] == limiter)
		{
			line = ft_gstrjoin(line, tmp, 1, 1);
			break ;
		}
		if (ft_strncmp(tmp, &limiter, 1) == 0 )
			break ;
		line = ft_gstrjoin(line, tmp, 1, 1);
	}
	return (line);
}

void	prompt(char **envp)
{
	t_list 	*list;
	
	list = malloc(sizeof(t_list ));
	while (1)
	{
		printf("%sMinishell:  %s", GREEN, RESET_COLOR);
		list->content = readline("");
		if (check_for_quotes(list->content, 34) == 2)
			list->content = quotes_handling(list->content, 34);
		if (check_for_quotes(list->content, 39) == 2)
			list->content = quotes_handling(list->content, 39);
		if (check_for_parenthesis(list->content) == 2)
			parenthesis_handling(list->content, &list);
		printf("%s\n", list->content);
	}
}