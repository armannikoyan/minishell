#include <stdbool.h>

#include "../../libs/libft/libft.h"

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
