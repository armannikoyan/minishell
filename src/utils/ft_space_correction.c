/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_space_correction.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 14:11:07 by anikoyan          #+#    #+#             */
/*   Updated: 2025/01/14 02:30:38 by gsimonia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	process_token(char *input, unsigned short *i, unsigned short *len)
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

static unsigned short	calculate_output_length(char *input)
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

static void	process_non_space_token(char *input, char *output,
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

static void	process_input_string(char *input, char *output)
{
	unsigned short	i;
	unsigned short	j;

	j = 0;
	i = 0;
	while (ft_isspace(input[i]))
		i++;
	while (input[i])
	{
		while (!ft_isspace(input[i]) && ft_strncmp(&input[i], "&", 1)
			&& !ft_isoperator(&input[i]) && input[i] != '\''
			&& input[i] && input[i] != '\"')
			process_non_space_token(input, output, &i, &j);
		if (input[i] && ft_isspace(input[i]))
		{
			if (output[j - 1] != ' ')
				output[j++] = ' ';
			while (input[i] && ft_isspace(input[i]))
				i++;
		}
		if (input[i] == '|' && input[i + 1] == '|')
		{
			if (j > 0 && output[j - 1] != ' ')
				output[j++] = ' ';
			output[j++] = '|';
			output[j++] = '|';
			if (input[i + 2] && !ft_isspace(input[i + 2]))
				output[j++] = ' ';
			i += 2;
		}
		else if ((input[i] == '>' && input[i + 1] == '>')
			|| (input[i] == '<' && input[i + 1] == '<'))
		{
			if (j > 0 && output[j - 1] != ' ')
				output[j++] = ' ';
			output[j++] = input[i];
			output[j++] = input[i + 1];
			i += 2;
			if (input[i] && !ft_isspace(input[i]))
				output[j++] = ' ';
		}
		else if (input[i] == '&')
		{
			if (j > 0 && output[j - 1] != ' ')
				output[j++] = ' ';
			output[j++] = '&';
			if (input[i + 1] == '&')
			{
				output[j++] = '&';
				i++;
			}
			if (input[i + 1] && !ft_isspace(input[i + 1]))
				output[j++] = ' ';
			i++;
		}
		else
		{
			ft_copy_quotes(output, input, &j, &i);
			handle_operator_copy(input, output, &i, &j);
		}
	}
	output[j] = '\0';
}

char	*ft_space_correction(char *input)
{
	char			*output;
	unsigned short	len;

	len = calculate_output_length(input);
	output = (char *)malloc(sizeof(char) * (len + 1));
	if (!output)
		return (NULL);
	process_input_string(input, output);
	return (output);
}
