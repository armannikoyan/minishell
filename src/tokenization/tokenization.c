/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasargsy <dasargsy@student.42yerevan.am    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 21:59:15 by dasargsy          #+#    #+#             */
/*   Updated: 2024/07/29 22:46:19 by dasargsy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static  char    *end_quote()
{
    
}


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
        if (line[i] == 39 || line[i] == 34)
        {
            limiter = line[i++];
            while (line[i] && line[i] != limiter)
                i++;
            if (!line[i])
                
        }
    }    
}

static  void    fill(char **tokens, char *line, int size)
{
    
}

static  char    **logic(char *line)
{
    int     size;
    int     i;
    char    **tokens;

    tokens = NULL;
    i = 0;
    size = get_size(line); 
    if (size == -1)
    {
    }
}

char    **tokenization(char *line)
{
    char    **tokens;
    
    if (!line)
        return (NULL);     
}