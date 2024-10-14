/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_space_correction.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 14:11:07 by anikoyan          #+#    #+#             */
/*   Updated: 2024/10/13 13:17:13 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	ft_skip_quotes(char *input, unsigned short *i, unsigned short *len)
{
	if (input[*i] == '\'')
	{
		(*i)++;
		(*len) += 2;
		while (input[*i] && input[*i] != '\'')
		{
			(*len)++;
			(*i)++;
		}
		(*i)++;
	}
	else if (input[*i] == '\"')
	{
		(*i)++;
		(*len) += 2;
		while (input[*i] && input[*i] != '\"')
		{
			(*len)++;
			(*i)++;
		}
		(*i)++;
	}
}

static void	ft_copy_quotes(char *output, char *input, unsigned short *j, unsigned short *i)
{
	if (input[*i] && input[*i] == '\'')
	{
		output[(*j)++] = input[(*i)++];
		while (input[*i] && input[*i] != '\'')
			output[(*j)++] = input[(*i)++];
		output[(*j)++] = input[(*i)++];
	}
	else if (input[*i] && input[*i] == '\"')
	{
		output[(*j)++] = input[(*i)++];
		while (input[*i] && input[*i] != '\"')
			output[(*j)++] = input[(*i)++];
		output[(*j)++] = input[(*i)++];
	}
}

void handle_operator_len(char *input, unsigned short *i, unsigned short *len)
{
	unsigned short op_len = ft_isoperator(&input[*i]);
	if (input[*i] && op_len)
	{
		if (*i > 0 && !ft_isspace(input[*i - 1]))
			(*len)++;
		(*len) += op_len;
		(*i) += op_len;

		op_len = ft_isoperator(&input[*i]);
		if (input[*i] && op_len)
		{
			(*len) += op_len;
			if (input[*i] && !ft_isspace(input[*i]))
				(*len)++;
			(*i) += op_len;
		}
		if (input[*i] && !ft_isspace(input[*i]))
			(*len)++;
	}
}

void handle_operator_copy(char *input, char *output, unsigned short *i, unsigned short *j)
{
	unsigned short op_len = ft_isoperator(&input[*i]);
	if (input[*i] && op_len)
	{
		if (*j > 0 && !ft_isspace(output[*j - 1]))
			output[(*j)++] = ' ';
		ft_strlcat(output + *j, input + *i, op_len + 1);
		*j += op_len;
		*i += op_len;
		output[(*j)++] = ' ';
		op_len = ft_isoperator(&input[*i]);
		if (input[*i] && op_len)
		{
			ft_strlcat(output + *j, input + *i, op_len + 1);
			*j += op_len;
			*i += op_len;
			if (input[*i] && !ft_isspace(input[*i]))
				output[(*j)++] = ' ';
		}
	}
}

char *ft_space_correction(char *input)
{
	char *output;
	unsigned short len = 0, i = 0, j = 0;

	while (input[i])
	{
		while (input[i] && !ft_isspace(input[i]) && !ft_isoperator(&input[i]))
			len++, i++;
		if (input[i] && ft_isspace(input[i]))
		{
			if (input[i - 1] != ' ')
				len++;
			while (input[i] && ft_isspace(input[i]))
				i++;
		}
		ft_skip_quotes(input, &i, &len);
		handle_operator_len(input, &i, &len);
	}

	output = (char *)malloc(sizeof(char) * (len + 1));
	i = 0, j = 0;

	while (input[i])
	{
		while (input[i] && !ft_isspace(input[i]) && !ft_isoperator(&input[i]) && input[i] != '\'' && input[i] != '\"')
			output[j++] = input[i++];
		if (input[i] && ft_isspace(input[i]))
		{
			if (output[j - 1] != ' ')
				output[j++] = input[i];
			while (input[i] && ft_isspace(input[i]))
				i++;
		}
		ft_copy_quotes(output, input, &j, &i);
		handle_operator_copy(input, output, &i, &j);
	}
	output[j] = '\0';
	ft_printf("len of output: %d\noutput: %s\n", len, output);
	return output;
}
