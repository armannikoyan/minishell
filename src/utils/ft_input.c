#include "../../includes/minishell.h"

static void	ft_print_tree(t_node *node)
{
	int	i;

	i = 0;
	if (!node)
		return ;
	ft_print_tree(node->left);
	ft_printf("------------\n");
	ft_printf("content:\n");
	while (node->content[i])
	{
		ft_printf("%s\n", node->content[i]);
		i++;
	}
	ft_printf("type: %c\n", node->type);
	ft_printf("------------\n");
	ft_print_tree(node->right);
}

void	handle_input(char *prompt, char **envp)
{
	char	*input;

	input = readline("Enter command: ");
	if (input && *input)
	{
		add_history(input);
		process_valid_input(input, envp);
	}
	else if (!input)
		clean_exit(prompt);
	free(prompt);
}

void	process_valid_input(char *input, char **envp)
{
	t_list	**lst;
	t_tree	*tree;

	lst = NULL;
	tree = NULL;
	input = process_input(input, envp);
	lst = tokenize_and_build_tree(input, &tree);
	if (lst && tree)
		ft_print_tree(tree->root);
	free_resources(input, lst, tree);
}

char	*process_input(char *input, char **envp)
{
	char	*tmp;

	tmp = input;
	input = ft_space_correction(tmp);
	free(tmp);
	tmp = input;
	input = ft_env_expansion(tmp, envp);
	free(tmp);
	return (input);
}

t_list	**tokenize_and_build_tree(char *input, t_tree **tree)
{
	t_list	**lst;

	lst = ft_tokenization(input);
	if (!lst)
		exit(EXIT_FAILURE);
	if (!ft_has_syntax_error(lst))
	{
		*tree = ft_tree_build(lst);
		if (!(*tree))
			exit(EXIT_FAILURE);
	}
	return (lst);
}
