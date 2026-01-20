#include <stdbool.h>
#include <stdio.h>

#include "hash_table.h"
#include "../../libs/libft/libft.h"
#include "builtin.h"

// Prints all passed arguments and finishes with status 0.
// In case of option '-n' passed doesn't add '\n' at the end.
int	ft_echo(int argc, char **argv, t_hash_table *ht)
{
	int		i;
	bool	n_flag;

	(void) ht;
	i = 1;
	n_flag = false;

	if (ft_strcmp(argv[i], "-n") == 0) {
		n_flag = true;
		i++;
	}
	while (i < argc) {
		printf("%s", argv[i]);
		if (i != argc - 1)
			printf(" ");
		i++;
	}
	if (!n_flag)
		printf("\n");
	return 0;
}
