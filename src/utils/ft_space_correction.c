/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_space_correction.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 14:11:07 by anikoyan          #+#    #+#             */
/*   Updated: 2024/11/18 18:38:15 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

// char	*ft_space_correction(char *input)
// {
// 	char			*output;
// 	unsigned short	len;
// 	unsigned short	i;
// 	unsigned short	j;

// 	len = 0;
// 	i = 0;
// 	j = 0;
// 	while (ft_isspace(input[i]))
// 		i++;
// 	while (input[i])
// 	{
// 		while (!ft_isspace(input[i]) && ft_strncmp(&input[i], "&", 1)
// 			&& !ft_isoperator(&input[i]) && input[i] != '\''
// 			&& input[i] && input[i] != '\"')
// 		{
// 			if (input[i] && input[i] == '('
// 				&& input[i + 1] && !ft_isspace(input[i]))
// 				len++;
// 			if (input[i] && input[i] == ')'
// 				&& input[i - 1] && !ft_isspace(input[i - 1]))
// 				len++;
// 			len++;
// 			i++;
// 		}
// 		if (input[i] && ft_isspace(input[i]))
// 		{
// 			if (input[i - 1] && input[i - 1] != ft_isspace(input[i]))
// 				len++;
// 			while (input[i] && ft_isspace(input[i]))
// 				i++;
// 		}
// 		ft_skip_quotes(input, &i, &len);
// 		handle_operator_len(input, &i, &len);
// 	}
// 	output = (char *)malloc(sizeof(char) * (len + 1));
// 	i = 0;
// 	j = 0;
// 	while (ft_isspace(input[i]))
// 		i++;
// 	while (input[i])
// 	{
// 		while (!ft_isspace(input[i]) && ft_strncmp(&input[i], "&", 1)
// 			&& !ft_isoperator(&input[i]) && input[i] != '\''
// 			&& input[i] && input[i] != '\"')
// 		{
// 			if (output[j - 1] && output[j - 1] != ' '
// 				&& input[i] && input[i] == ')')
// 				output[j++] = ' ';
// 			output[j++] = input[i++];
// 			if (output[j - 1] && output[j - 1] == '('
// 				&& input[i] && input[i] != ft_isspace(input[i]))
// 				output[j++] = ' ';
// 		}
// 		if (input[i] && ft_isspace(input[i]))
// 		{
// 			if (output[j - 1] != ' ')
// 				output[j++] = input[i];
// 			while (input[i] && ft_isspace(input[i]))
// 				i++;
// 		}
// 		ft_copy_quotes(output, input, &j, &i);
// 		handle_operator_copy(input, output, &i, &j);
// 	}
// 	output[j] = '\0';
// 	return (output);
// }

char	*ft_space_correction(char *input)
{
	char			*output;
	unsigned short	len;
	unsigned short	j;

	j = 0;
	len = calculate_corrected_length(input);
	output = (char *)malloc(sizeof(char) * (len + 1));
	if (!output)
		return (NULL);
	j = copy_corrected_output(output, input);
	output[j] = '\0';
	return (output);
}

unsigned short	copy_corrected_output(char *output, char *input)
{
	unsigned short	i;
	unsigned short	j;

	i = 0;
	j = 0;
	skip_leading_spaces(input, &i);
	while (input[i])
	{
		if (!ft_isspace(input[i]) && !is_special_character(input[i]))
		{
			handle_space_after_parentheses(output, input, &i, &j);
			output[j++] = input[i++];
		}
		else if (ft_isspace(input[i]))
		{
			if (output[j - 1] != ' ')
				output[j++] = ' ';
			skip_leading_spaces(input, &i);
		}
		ft_copy_quotes(output, input, &j, &i);
		handle_operator_copy(input, output, &i, &j);
	}
	return (j);
}
