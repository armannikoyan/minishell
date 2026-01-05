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
		|| !ft_strncmp(str, "&&", 2) || *str == '(' || *str == ')')
		return (true);
	return (false);
}

size_t	get_operator_len(t_node_type type)
{
	if (type == HEREDOC_NODE || type == REDIRECT_APPEND_NODE
			|| type == OR_NODE || type == AND_NODE)
		return (2);
	else if (type == REDIRECT_IN_NODE || type == REDIRECT_OUT_NODE
			|| type == PIPE_NODE)
		return (1);
	return (0);
}

t_node_type	get_node_type(char *input)
{
	if (!ft_strncmp(input, "<<", 2))
		return (HEREDOC_NODE);
	else if (*input == '<')
		return (REDIRECT_IN_NODE);
	else if (!ft_strncmp(input, ">>", 2))
		return (REDIRECT_APPEND_NODE);
	else if (*input == '>')
		return (REDIRECT_OUT_NODE);
	else if (!ft_strncmp(input, "||", 2))
		return (OR_NODE);
	else if (*input == '|')
		return (PIPE_NODE);
	else if (!ft_strncmp(input, "&&", 2))
		return (AND_NODE);
	else if (*input == '(')
		return (SUBSHELL_NODE);
	else if (*input == ')')
		return (ERROR_NODE);
	return (COMMAND_NODE);
}