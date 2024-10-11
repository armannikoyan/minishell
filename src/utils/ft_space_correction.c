/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_space_correction.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 14:11:07 by anikoyan          #+#    #+#             */
/*   Updated: 2024/10/11 15:17:55 by anikoyan         ###   ########.fr       */
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
	}
	else if (input[*i] && input[*i] == '\"')
	{
		output[(*j)++] = input[(*i)++];
		while (input[*i] && input[*i] != '\"')
			output[(*j)++] = input[(*i)++];
	}
}

char	*ft_space_correction(char *input)
{
	char		*output;
	unsigned short	len;
	unsigned short	i;
	unsigned short	j;
	unsigned short	op_len;

	len = 0;
	i = 0;
	op_len = 0;
	while (input[i])
	{
		while (input[i] && !ft_isspace(input[i]) && !ft_isoperator(&input[i]))
		{
			len++;
			i++;
		}
		if (input[i] && ft_isspace(input[i]))
		{
			len++;
			while (input[i] && ft_isspace(input[i]))
				i++;
		}
		ft_skip_quotes(input, &i, &len);
		op_len = ft_isoperator(&input[i]);
		if (input[i] && op_len)
		{
			if (input[i - 1] && !ft_isspace(input[i - 1]))
				len++;
			len += op_len;
			i += op_len;
			op_len = ft_isoperator(&input[i]);
			if (input[i] && op_len)
			{
				len += op_len;
				if (input[i] && !ft_isspace(input[i]))
					len++;
				i += op_len;
			}
			if (input[i] && !ft_isspace(input[i]))
				len++;
		}
	}
	output = (char *)malloc(sizeof(char) * (len + 1));
	i = 0;
	j = 0;
	while (input[i])
	{
		while (input[i] && !ft_isspace(input[i]) && !ft_isoperator(&input[i]) && input[i] != '\'' && input[i] != '\"')
			output[j++] = input[i++];
		if (input[i] && ft_isspace(input[i]))
		{
			output[j++] = input[i];
			while (input[i] && ft_isspace(input[i]))
				i++;
		}
		ft_copy_quotes(output, input, &j, &i);
		op_len = ft_isoperator(&input[i]);
		if (input[i] && op_len)
		{
			if (input[i - 1] && !ft_isspace(input[i - 1]))
				output[j++] = ' ';
			ft_strlcat(output + j, input + i, op_len + 1);
			j += op_len;
			i += op_len;
			op_len = ft_isoperator(&input[i]);
			output[j++] = ' ';
			if (input[i] && op_len)
			{
				ft_strlcat(output + j, input + i, op_len + 1);
				j += op_len;
				i += op_len;
				if (input[i] && !ft_isspace(input[i]))
					output[j++] = ' ';
			}
		}
	}
	output[j] = '\0';
	ft_printf("output is: %s\n", output);
	return (output);
}
