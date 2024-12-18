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

void	handle_space_after_parentheses(char *output,
		char *input, unsigned short *i, unsigned short *j)
{
	if (output[*j - 1] && output[*j - 1] != ' ' && input[*i] == ')')
		output[(*j)++] = ' ';
	if (output[*j - 1] == '(' && input[*i] != '\0' && !ft_isspace(input[*i]))
		output[(*j)++] = ' ';
}

unsigned short	calculate_corrected_length(char *input)
{
	unsigned short	len;
	unsigned short	i;

	len = 0;
	i = 0;
	skip_leading_spaces(input, &i);
	while (input[i])
	{
		if (!ft_isspace(input[i]) && !is_special_character(input[i]))
		{
			handle_parentheses_length(input, &i, &len);
			handle_general_characters(input, &i, &len);
		}
		else if (ft_isspace(input[i]))
			handle_spaces(input, &i, &len);
		ft_skip_quotes(input, &i, &len);
		handle_operator_len(input, &i, &len);
	}
	return (len);
}
