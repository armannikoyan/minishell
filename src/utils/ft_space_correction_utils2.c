/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_space_correction_utils2.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 23:47:31 by gsimonia          #+#    #+#             */
/*   Updated: 2025/01/15 23:48:23 by gsimonia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	process_token(char *input, unsigned short *i, unsigned short *len)
{
	if (input[*i] == '(' && input[*i + 1] && !ft_isspace(input[*i]))
		(*len)++;
	if (input[*i] == ')' && input[*i - 1] && !ft_isspace(input[*i - 1]))
		(*len)++;
	if ((input[*i] == '|' && input[*i + 1] == '|')
		|| (input[*i] == '>' && input[*i + 1] == '>')
		|| (input[*i] == '<' && input[*i + 1] == '<'))
	{
		(*i)++;
		(*len)++;
	}
	else if (input[*i] == '&')
	{
		(*len)++;
		while (input[*i + 1] == '&' || input[*i + 1] == '&')
		{
			(*i)++;
			(*len)++;
		}
	}
	else
		(*len)++;
	(*i)++;
}

unsigned short	calculate_output_length(char *input)
{
	unsigned short	len;
	unsigned short	i;

	len = 0;
	i = 0;
	while (ft_isspace(input[i]))
		i++;
	while (input[i])
	{
		while (!ft_isspace(input[i]) && ft_strncmp(&input[i], "&", 1)
			&& !ft_isoperator(&input[i]) && input[i] != '\''
			&& input[i] && input[i] != '\"')
			process_token(input, &i, &len);
		if (input[i] && ft_isspace(input[i]))
		{
			if (input[i - 1] && input[i - 1] != ft_isspace(input[i]))
				len++;
			while (input[i] && ft_isspace(input[i]))
				i++;
		}
		ft_skip_quotes(input, &i, &len);
		handle_operator_len(input, &i, &len);
	}
	return (len);
}

void	process_non_space_token(char *input, char *output,
		unsigned short *i, unsigned short *j)
{
	if (*j > 0 && output[*j - 1] != ' ' && output[*j - 1] != '('
		&& input[*i] == ')')
		output[(*j)++] = ' ';
	if (ft_isoperator(&input[*i]) && *j > 0 && output[*j - 1] != ' '
		&& output[*j - 1] != '(')
		output[(*j)++] = ' ';
	output[(*j)++] = input[(*i)++];
	if (output[*j - 1] == '(' && input[*i] && !ft_isspace(input[*i])
		&& input[*i] != ')')
		output[(*j)++] = ' ';
}

void	skip_initial_spaces(char *input, unsigned short *i)
{
	while (ft_isspace(input[*i]))
		(*i)++;
}

void	handle_double_pipe(char *output,
		unsigned short *j, unsigned short *i)
{
	if (*j > 0 && output[*j - 1] != ' ')
		output[(*j)++] = ' ';
	output[(*j)++] = '|';
	output[(*j)++] = '|';
	if (*(i + 2) && !ft_isspace(*(i + 2)))
		output[(*j)++] = ' ';
	*i += 2;
}
