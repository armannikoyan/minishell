#include "../../includes/minishell.h"

int	operator_precedence(char *op)
{
	if (ft_strcmp(op, "&&") == 0 || ft_strcmp(op, "||") == 0)
		return (3); // Lowest precedence
	if (ft_strcmp(op, "|") == 0)
		return (2);
	if (ft_strcmp(op, "<") == 0 || ft_strcmp(op, ">") == 0
		|| ft_strcmp(op, ">>") == 0 || ft_strcmp(op, "<<") == 0)
		return (1); // Highest precedence
	return (0);// Default for unknown
}

int	operator_has_higher_precedence(t_node *new_op, t_node *current_op)
{
	int	new_prec;
	int	current_prec;

	new_prec = operator_precedence(new_op->content[0]);
	current_prec = operator_precedence(current_op->content[0]);
	// Higher precedence operator replaces current
	return (new_prec > current_prec);
}

static void	ft_add_operator_node(t_tree *tree, t_node *node)
{
	t_node	*current;

	*current = tree->root;
	if (operator_has_higher_precedence(node, current))
	{
		node->left = current;
		tree->root = node;
	}
	else
	{
		while (current->right)
			current = current->right;
		current->right = node;
	}
}

static void	ft_add_non_operator_node(t_tree *tree, t_node *node)
{
	t_node	*current;

	*current = tree->root;
	while (current->right)
		current = current->right;
	if (!current->left)
		current->left = node;
	else
		current->right = node;
}

static void	add_to_tree(t_tree *tree, t_node *node)
{
	if (!tree || !node)
		return ;
	if (!tree->root)
	{
		tree->root = node;
		return ;
	}
	if (node->type == 'O')
		ft_add_operator_node(tree, node);
	else
		ft_add_non_operator_node(tree, node);
}
