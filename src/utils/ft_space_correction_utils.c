#include "../../includes/minishell.h"

void	ft_skip_quotes(char *input, unsigned short *i, unsigned short *len)
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

void	ft_copy_quotes(char *output, char *input,
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

void	handle_operator_len(char *input, unsigned short *i,
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

void	handle_operator_copy(char *input, char *output,
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
