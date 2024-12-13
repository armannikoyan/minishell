/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_syntax_tree.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 12:47:00 by anikoyan          #+#    #+#             */
/*   Updated: 2024/12/13 12:56:30 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static t_tree	*ft_tree_ctor()
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

static unsigned short	ft_contentlen(t_list **lst)
{
	unsigned short	len;
	t_list			*tmp;

	len = 0;
	tmp = *lst;
	if ((t_token *)tmp->content && ((t_token *)tmp->content)->type == 'X')
	{
		len++;
		tmp = tmp->next;
		while (tmp && (t_token *)tmp->content && ((t_token *)tmp->content)->type != 'O')
		{
			len++;
			tmp = tmp->next;
		}
		while (tmp && (t_token *)tmp->content && (((t_token *)tmp->content)->type == 'O'
					|| ((t_token *)tmp->content)->type == 'F'))
			tmp = tmp->next;
		while (tmp && (t_token *)tmp->content && ((t_token *)tmp->content)->type == 'A')
		{
			len++;
			tmp = tmp->next;
		}
	}
	else if ((t_token *)tmp->content && ((t_token *)tmp->content)->type == 'O')
	{
		if (((t_token *)tmp->content)->type == 'O'
				&& ft_strlen(((t_token *)tmp->content)->content) == 2
				&& (((t_token *)tmp->content)->content[0] == '>'
					|| ((t_token *)tmp->content)->content[0] == '<'))
			return (2);
		else
			return (1);
	}
	return (len);
}

static t_node	*parse_node(t_list **lst)
{
	char			**content;
	char			type_of_node;
	t_list			*tmp;
	unsigned short	i;

	if (!lst || !*lst)
		exit(EXIT_FAILURE);
	type_of_node = 0;
	i = 0;
	tmp = *lst;
	content = (char **)malloc(sizeof(char *) * (ft_contentlen(&tmp) + 1));
	if (!content)
		exit(EXIT_FAILURE);
	if ((t_token *)tmp->content && ((t_token *)tmp->content)->type == 'X')
	{
		type_of_node = 'X';
		while (tmp && (t_token *)tmp->content && ((t_token *)tmp->content)->type != 'O')
		{
			content[i++] = ((t_token *)tmp->content)->content;
			tmp = tmp->next;
		}
		while (tmp && (t_token *)tmp->content && (((t_token *)tmp->content)->type == 'O'
				|| ((t_token *)tmp->content)->type == 'F'))
			tmp = tmp->next;
		while (tmp && (t_token *)tmp->content && ((t_token *)tmp->content)->type == 'A')
		{
			content[i++] = ((t_token *)tmp->content)->content;
			tmp = tmp->next;
		}
	}
	else if ((t_token *)tmp->content && ((t_token *)tmp->content)->type == 'O')
	{
		type_of_node = 'O';
		while (tmp && (t_token *)tmp->content && (((t_token *)tmp->content)->type == 'O'
				|| ((t_token *)tmp->content)->type == 'F'))
		{
			content[i++] = ((t_token *)tmp->content)->content;
			tmp = tmp->next;
		}
	}
	else
	{
		free(content);
		return (NULL);
	}
	return (ft_node_ctor(content, type_of_node));
}

int operator_precedence(char *op) {
	if (ft_strcmp(op, "&&") == 0 || ft_strcmp(op, "||") == 0)
		return 1; // Lowest precedence
	if (ft_strcmp(op, "|") == 0)
		return 2;
	if (ft_strcmp(op, "<") == 0 || ft_strcmp(op, ">") == 0
			|| ft_strcmp(op, ">>") == 0 || ft_strcmp(op, "<<") == 0)
		return 3; // Highest precedence
	return 0; // Default for unknown
}

int operator_has_higher_precedence(t_node *new_op, t_node *current_op) {
	int new_prec = operator_precedence(new_op->content[0]);
	int current_prec = operator_precedence(current_op->content[0]);

	// Higher precedence operator replaces current
	return new_prec > current_prec;
}

static void add_to_tree(t_tree *tree, t_node *node)
{
	t_node	*current;

	if (!tree || !node)
		return ;
	if (!tree->root)
	{
		tree->root = node;
		return ;
	}
	current = tree->root;
	if (node->type == 'O')
	{
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
	else
	{
		while (current->right)
			current = current->right;
		if (!current->left)
			current->left = node;
		else
			current->right = node;
	}
}

t_tree	*ft_tree_build(t_list **lst)
{
	t_tree	*tree;
	t_list	*tmp;
	t_node	*new_node;

	tree = ft_tree_ctor();
	tmp = *lst;
	if (!tree || !lst || !*lst)
		return (NULL);
	while (tmp)
	{
		new_node = parse_node(&tmp);
		if (new_node)
			add_to_tree(tree, new_node);
		tmp = tmp->next;
	}
	ft_tree_dtor(NULL);
	return (tree);
}
