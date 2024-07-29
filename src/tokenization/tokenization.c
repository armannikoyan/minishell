/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasargsy <dasargsy@student.42yerevan.am    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 21:59:15 by dasargsy          #+#    #+#             */
/*   Updated: 2024/07/29 23:58:35 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static  int     get_size(char *line)
{
    int     i;
    int     size;
    char    limiter;

    i = 0;
    size = 1;
    limiter = 0;
    while (line[i])
    {
		if (line[i] == 39 || line[i] == 34 || line[i] == 96)
		{
			if (!is_closed(line + i + 1, line[i]))
				return (0);
		}
        if (line[i] == 39 || line[i] == 34)
        {
            limiter = line[i++];
            while (line[i] && line[i] != limiter)
                i++;
        }
        if (ft_isspace(line[i]) && !ft_isspace(line[i + 1]))
            size++;
    }
    return (size);    
}

static  void    fill(char **tokens, char *line, int size)
{
	return ; // TODO
}

static  char    **logic(char *line)
{
    int     size;
    int     i;
    char    **tokens;

    tokens = NULL;
    i = 0;
    size = get_size(line); 
    if (!size)
        quote_error(line);
    fill(tokens, line, size);
}

char    **tokenization(char *line)
{
    char    **tokens;
    
    if (!line)
        return (NULL);     
    return (logic(line));
}
