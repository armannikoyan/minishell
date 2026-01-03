#include <stdbool.h>

#include "../../libs/libft/libft.h"
#include "../../includes/ast.h"

bool	is_redir(char *str)
{
	if (!ft_strncmp(str, "<<", 2) || *str == '<'
		|| !ft_strncmp(str, ">>", 2) || *str == '>')
		return (true);
	return (false);
}

bool	is_operator(char *str)
{
	if (!ft_strncmp(str, "||", 2) || *str == '|'
		|| !ft_strncmp(str, "&&", 2))
		return (true);
	return (false);
}

t_node_type	get_node_type(char *input)
{
	if (!ft_strncmp(input, "<<", 2))
		return (NODE_HEREDOC);
	else if (*input == '<')
		return (NODE_REDIRECT_IN);
	else if (!ft_strncmp(input, ">>", 2))
		return (NODE_REDIRECT_APPEND);
	else if (*input == '>')
		return (NODE_REDIRECT_OUT);
	else if (!ft_strncmp(input, "||", 2))
		return (NODE_OR);
	else if (*input == '|')
		return (NODE_PIPE);
	else if (!ft_strncmp(input, "&&", 2))
		return (NODE_AND);
	return (NODE_COMMAND);
}

size_t	get_operator_len(t_node_type type)
{
	if (type == NODE_HEREDOC || type == NODE_REDIRECT_APPEND
			|| type == NODE_OR || type == NODE_AND)
		return (2);
	else if (type == NODE_REDIRECT_IN || type == NODE_REDIRECT_OUT
			|| type == NODE_PIPE)
		return (1);
	return (0);
}
