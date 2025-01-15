/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_space_correctiion_utils3.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 23:49:59 by gsimonia          #+#    #+#             */
/*   Updated: 2025/01/15 23:54:38 by gsimonia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_double_redir(char *input, char *output,
		unsigned short *i, unsigned short *j)
{
	if (*j > 0 && output[*j - 1] != ' ')
		output[(*j)++] = ' ';
	output[(*j)++] = input[*i];
	output[(*j)++] = input[*i + 1];
	*i += 2;
	if (input[*i] && !ft_isspace(input[*i]))
		output[(*j)++] = ' ';
}

void	handle_ampersand(char *input, char *output,
		unsigned short *i, unsigned short *j)
{
	if (*j > 0 && output[*j - 1] != ' ')
		output[(*j)++] = ' ';
	output[(*j)++] = '&';
	if (input[*i + 1] == '&')
	{
		output[(*j)++] = '&';
		(*i)++;
	}
	if (input[*i + 1] && !ft_isspace(input[*i + 1]))
		output[(*j)++] = ' ';
	(*i)++;
}
