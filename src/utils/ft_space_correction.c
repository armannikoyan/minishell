/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_space_correction.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 14:11:07 by anikoyan          #+#    #+#             */
/*   Updated: 2025/01/13 05:34:01 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

static void	process_non_space_token(char *input, char *output, unsigned short *i, unsigned short *j)
{
	if (*j > 0 && output[*j - 1] != ' ' && output[*j - 1] != '(' && input[*i] == ')')
		output[(*j)++] = ' ';
	if (ft_isoperator(&input[*i]) && *j > 0 && output[*j - 1] != ' ' && output[*j - 1] != '(')
		output[(*j)++] = ' ';
	output[(*j)++] = input[(*i)++];
	if (output[*j - 1] == '(' && input[*i] && !ft_isspace(input[*i]) && input[*i] != ')')
		output[(*j)++] = ' ';
}

static void process_input_string(char *input, char *output)
{
	unsigned short i;
	unsigned short j;

	j = 0;
	i = 0;
	while (ft_isspace(input[i]))  // Skip initial spaces
		i++;
	while (input[i])
	{
		// Process tokens that are not operators or spaces
		while (!ft_isspace(input[i]) && ft_strncmp(&input[i], "&", 1)
				&& !ft_isoperator(&input[i]) && input[i] != '\''
				&& input[i] && input[i] != '\"')
			process_non_space_token(input, output, &i, &j);

		// Handle space before or after operators (like pipe)
		if (input[i] && ft_isspace(input[i]))
		{
			if (output[j - 1] != ' ')  // Prevent consecutive spaces
				output[j++] = input[i];
			while (input[i] && ft_isspace(input[i]))
				i++;
		}

		// Special handling for pipe operator (|), redirections (<, >)
		if (input[i] == '|')
		{
			if (j > 0 && output[j - 1] != ' ')
				output[j++] = ' ';  // Ensure space before pipe
			output[j++] = '|';  // Add pipe itself
			if (input[i + 1] && !ft_isspace(input[i + 1]))
				output[j++] = ' ';  // Ensure space after pipe
			i++;
		}
		else if ((input[i] == '<' && input[i + 1] == '<') || (input[i] == '>' && input[i + 1] == '>'))
		{
			// Handle << and >> operators as single tokens
			if (j > 0 && output[j - 1] != ' ')
				output[j++] = ' ';  // Ensure space before redirection
			output[j++] = input[i];  // Add the first character of the operator
			output[j++] = input[i + 1];  // Add the second character of the operator
			i += 2;  // Skip the next character
			// Ensure space after redirection operator if next token is not a space
			if (input[i] && !ft_isspace(input[i]))
				output[j++] = ' ';
		}
		else if (input[i] == '<' || input[i] == '>')
		{
			// Handle single < or > as redirection
			if (j > 0 && output[j - 1] != ' ')
				output[j++] = ' ';  // Ensure space before redirection
			output[j++] = input[i];  // Add redirection operator itself
			i++;
			// Ensure space after redirection operator if next token is not a space
			if (input[i] && !ft_isspace(input[i]))
				output[j++] = ' ';
		}
		else
		{
			// Copy the token to output as is
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
