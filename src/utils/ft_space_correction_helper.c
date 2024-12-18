#include "../../includes/minishell.h"

void	skip_leading_spaces(char *input, unsigned short *i)
{
	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
}

void	handle_spaces(char *input, unsigned short *i, unsigned short *len)
{
	if (*i > 0 && !ft_isspace(input[*i - 1]))
		(*len)++;
	skip_leading_spaces(input, i);
}

unsigned short	handle_general_characters(char *input,
		unsigned short *i, unsigned short *len)
{
	(*len)++;
	(*i)++;
	return (*len);
}

void	handle_parentheses_length(char *input,
		unsigned short *i, unsigned short *len)
{
	if (input[*i] == '(' && input[*i + 1] && !ft_isspace(input[*i + 1]))
		(*len)++;
	if (input[*i] == ')' && *i > 0 && !ft_isspace(input[*i - 1]))
		(*len)++;
}

bool	is_special_character(char c)
{
	return (ft_strncmp(&c, "&", 1) == 0
		|| ft_isoperator(&c) || c == '\'' || c == '\"');
}
