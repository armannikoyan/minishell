#include "../../includes/minishell.h"

int	write_error(const char *prefix, const char *message, const char *arg)
{
	if (prefix)
		write(2, prefix, ft_strlen(prefix));
	if (message)
		write(2, message, ft_strlen(message));
	if (arg)
	{
		write(2, arg, ft_strlen(arg));
		write(2, "\n", 1);
	}
	return (EXIT_FAILURE);
}

void	write_string(const char *str1, const char *str2, const char *str3)
{
	if (str1)
		write(1, str1, ft_strlen(str1));
	if (str2)
		write(1, str2, ft_strlen(str2));
	if (str3)
		write(1, str3, ft_strlen(str3));
}
