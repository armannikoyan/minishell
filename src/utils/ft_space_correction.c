/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_space_correction.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 14:11:07 by anikoyan          #+#    #+#             */
/*   Updated: 2024/12/25 17:56:46 by anikoyan         ###   ########.fr       */
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

static void	ft_copy_quotes(char *output, char *input,
	unsigned short *j, unsigned short *i)
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

static void	handle_operator_len(char *input, unsigned short *i,
	unsigned short *len)
{
	unsigned short	op_len;

	op_len = ft_isoperator(&input[*i]);
	if (!op_len)
		op_len = !ft_strncmp(&input[*i], "&", 1);
	if (input[*i] && op_len)
	{
		if (*i > 0 && !ft_isspace(input[*i - 1]))
			(*len)++;
		(*len) += op_len;
		(*i) += op_len;
		if (*i > 0 && !ft_isspace(input[*i]) && !ft_isoperator(&input[*i])
			&& ft_strncmp(&input[*i], "&", 1))
			(*len)++;
	}
}

static void	handle_operator_copy(char *input, char *output,
	unsigned short *i, unsigned short *j)
{
	unsigned short	op_len;

	op_len = ft_isoperator(&input[*i]);
	if (!op_len)
		op_len = !ft_strncmp(&input[*i], "&", 1);
	if (input[*i] && op_len)
	{
		if (*j > 0 && !ft_isspace(output[*j - 1]))
			output[(*j)++] = ' ';
		ft_strlcat(&output[*j], &input[*i], op_len + 1);
		*j += op_len;
		*i += op_len;
		if (*j > 0 && !ft_isspace(output[*j - 1]))
			output[(*j)++] = ' ';
	}
}

static void	process_token(char *input, unsigned short *i, unsigned short *len)
{
	if (input[*i] == '(' && input[*i + 1] && !ft_isspace(input[*i]))
		(*len)++;
	if (input[*i] == ')' && input[*i - 1] && !ft_isspace(input[*i - 1]))
		(*len)++;
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

static void	process_non_space_token(char *input,
		char *output, unsigned short *i, unsigned short *j)
{
	if (*j > 0 && output[*j - 1] != ' '
		&& output[*j - 1] != '(' && input[*i] == ')')
		output[(*j)++] = ' ';
	output[(*j)++] = input[(*i)++];
	if (output[*j - 1] == '('
		&& input[*i] && !ft_isspace(input[*i]) && input[*i] != ')')
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
				output[j++] = input[i];
			while (input[i] && ft_isspace(input[i]))
				i++;
		}
		ft_copy_quotes(output, input, &j, &i);
		handle_operator_copy(input, output, &i, &j);
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
