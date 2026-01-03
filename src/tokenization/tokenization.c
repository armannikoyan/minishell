#include <stddef.h>
#include <stdlib.h>

#include "../../includes/tokenization.h"
#include "../../libs/libft/libft.h"

static void	set_quote_char(char c, char *quote_char)
{
	if (c == '"' && *quote_char != '\'')
	{
		if (*quote_char == '"')
			*quote_char = 0;
		else
			*quote_char = '"';
	}
	else if (c == '\'' && *quote_char != '"')
	{
		if (*quote_char == '\'')
			*quote_char = 0;
		else
			*quote_char = '\'';
	}
}

static char	*substr_next(char *input, size_t *i)
{
	char	*tmp;
	size_t	start;
	size_t	j;
	char	quote_char;

	while (input[*i] == ' ')
		(*i)++;
	start = *i;
	quote_char = 0;
	while (input[*i])
	{
		set_quote_char(input[*i], &quote_char);
		if (quote_char == 0 && (input[*i] == ' ' ||
			is_operator(&(input[*i])) || is_redir(&(input[*i]))))
			break ;
		(*i)++;
	}
	tmp = (char *)malloc(sizeof(char) * ((*i - start) + 1));
	if (!tmp)
		return (NULL);
	j = 0;
	while (start < *i)
		tmp[j++] = input[start++];
	tmp[j] = '\0';
	return (tmp);
}

static char	**get_argv(char *input, size_t *i)
{
	char	**argv;
	char	quote_char;
	size_t	len;
	size_t	j;

	j = *i;
	len = 0;
	while (input[j] && !is_operator(&(input[j])) && !is_redir(&(input[j])))
	{
		while (input[j] == ' ')
			j++;
		if (input[j] && !is_operator(&(input[j])) && !is_redir(&(input[j])))
		{
			len++;
			quote_char = 0;
			while (input[j])
			{
				set_quote_char(input[j], &quote_char);
				if (quote_char == 0 && (input[j] == ' ' ||
					is_operator(&(input[j])) || is_redir(&(input[j]))))
					break ;
				j++;
			}
		}
	}
	if (len == 0)
		return (NULL);
	argv = (char **)malloc(sizeof(char *) * (len + 1));
	if (!argv)
		return (NULL);
	j = 0;
	while (j < len)
		argv[j++] = substr_next(input, i);
	argv[len] = NULL;
	return (argv);
}

t_ast_node	*tokenize(char *input)
{
	t_ast_node	*node;
	t_node_type	type;
	size_t	i;
	size_t	op_len;

	i = 0;
	node = NULL;

	while (input[i])
	{
		while (input[i] == ' ')
			++i;
		if (!input[i])
			break ;
		type = get_node_type(&(input[i]));
		if (type == NODE_COMMAND)
			node = create_cmd_node(NODE_COMMAND, get_argv(input, &i));
		else if (is_operator(&(input[i])) || is_redir(&(input[i])))
		{
			op_len = get_operator_len(type);
			if (type >= NODE_REDIRECT_IN && type <= NODE_HEREDOC)
			{
				i += op_len; 
				node = create_redir_node(type, substr_next(input, &i), -1);
			}
			else
			{
				i += op_len;
				node = create_binary_node(type);
			}
		}
		if (node)
			node = ast_build(node);
	}
	return (node);
}
