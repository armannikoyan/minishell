#include "../../includes/minishell.h"

extern void	rl_clear_history(void);

void	clean_exit(char *prompt)
{
	free(prompt);
	rl_clear_history();
	exit(EXIT_SUCCESS);
}

void	free_resources(char *input, t_list **lst, t_tree *tree)
{
	free(input);
	if (lst)
	{
		ft_lstclear(lst, ft_tokendelone);
		free(lst);
	}
	(void)tree;
}
