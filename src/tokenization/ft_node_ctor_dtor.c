#include "../../includes/minishell.h"

static t_node	*ft_node_ctor(char **content, char type)
{
	t_node	*new;

	new = (t_node *)malloc(sizeof(t_node));
	if (!new)
		return (NULL);
	new->content = content;
	new->type = type;
	new->left = NULL;
	new->right = NULL;
	return (new);
}

static void	ft_node_dtor(t_node **node)
{
	if (!node || !*node)
		return ;
	ft_free_2d_array((void ***)(&(*node)->content));
	free(*node);
	*node = NULL;
}

static t_tree	*ft_tree_ctor(void)
{
	t_tree	*new;

	new = (t_tree *)malloc(sizeof(t_tree));
	if (!new)
		return (NULL);
	new->root = NULL;
	return (new);
}

static void	ft_tree_dtor(t_tree **tree)
{
	t_node	*current;

	if (!tree || !*tree)
		return ;
	current = (*tree)->root;
	while (current)
	{
		if (current->left)
			ft_node_dtor(&current->left);
		if (current->right)
			ft_node_dtor(&current->right);
		ft_node_dtor(&current);
	}
	free(*tree);
	*tree = NULL;
}
