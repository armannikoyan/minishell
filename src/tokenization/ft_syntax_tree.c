/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_syntax_tree.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 12:47:00 by anikoyan          #+#    #+#             */
/*   Updated: 2024/12/31 22:44:58 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_node	*ft_node_ctor(char **content, char type, unsigned short subshell_level)
{
	t_node	*new;

	new = (t_node *)malloc(sizeof(t_node));
	if (!new)
		return (NULL);
	new->content = content;
	new->type = type;
	new->subshell_level = subshell_level;
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

void	ft_tree_dtor(t_tree **tree)
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

static unsigned short	handle_o_token(t_list *tmp)
{
	if ((t_token *)tmp->content && ((t_token *)tmp->content)->type == 'O')
	{
		if ((((t_token *)tmp->content)->content[0] == '>'
				|| ((t_token *)tmp->content)->content[0] == '<'))
			return (2);
		else
			return (1);
	}
	return (0);
}

static void	increment_len_and_move(t_list **tmp, unsigned short *len)
{
	(*len)++;
	*tmp = (*tmp)->next;
}

static unsigned short	ft_contentlen(t_list **lst)
{
	unsigned short	len;
	t_list			*tmp;

	len = 0;
	tmp = *lst;
	if ((t_token *)tmp->content && ((t_token *)tmp->content)->type == 'X')
	{
		increment_len_and_move(&tmp, &len);
		while (tmp && (t_token *)tmp->content
			&& ((t_token *)tmp->content)->type != 'O')
			increment_len_and_move(&tmp, &len);
		while (tmp && (t_token *)tmp->content
			&& (((t_token *)tmp->content)->type == 'O'
				|| ((t_token *)tmp->content)->type == 'F'))
			tmp = tmp->next;
		while (tmp && (t_token *)tmp->content
			&& ((t_token *)tmp->content)->type == 'A')
			increment_len_and_move(&tmp, &len);
	}
	else if ((t_token *)tmp->content && ((t_token *)tmp->content)->type == 'O')
		return (handle_o_token(tmp));
	return (len);
}

static void	parse_x_node(t_list **lst,
		char ***content, char *type_of_node)
{
	t_list	*tmp;
	unsigned short	i;

	*type_of_node = 'X';
	tmp = *lst;
	i = 0;
	while (tmp && (t_token *)tmp->content
		&& ((t_token *)tmp->content)->type != 'O')
	{
		(*content)[i++] = ((t_token *)tmp->content)->content;
		tmp = tmp->next;
	}
	while (tmp && (t_token *)tmp->content
		&& (((t_token *)tmp->content)->type == 'O'
			|| ((t_token *)tmp->content)->type == 'F'))
		tmp = tmp->next;
	while (tmp && (t_token *)tmp->content
		&& ((t_token *)tmp->content)->type == 'A')
	{
		(*content)[i++] = ((t_token *)tmp->content)->content;
		tmp = tmp->next;
	}
}

static void	parse_o_node(t_list **lst,
		char ***content, char *type_of_node)
{
	t_list	*tmp;
	unsigned short	i;

	*type_of_node = 'O';
	tmp = *lst;
	i = 0;
	(*content)[i++] = ((t_token *)tmp->content)->content;
	tmp = tmp->next;
	if (tmp && (t_token *)tmp->content
		&& ((t_token *)tmp->content)->type == 'F')
	{
		(*content)[i++] = ((t_token *)tmp->content)->content;
		tmp = tmp->next;
	}
}

static t_node	*parse_node(t_list **lst)
{
	char			**content;
	char			type_of_node;
	t_list			*tmp;

	if (!lst || !*lst)
		exit(EXIT_FAILURE);
	tmp = *lst;
	content = (char **)malloc(sizeof(char *) * (ft_contentlen(&tmp) + 1));
	if (!content)
		exit(EXIT_FAILURE);
	if ((t_token *)tmp->content && ((t_token *)tmp->content)->type == 'X')
		parse_x_node(lst, &content, &type_of_node);
	else if ((t_token *)tmp->content && ((t_token *)tmp->content)->type == 'O')
		parse_o_node(lst, &content, &type_of_node);
	else
	{
		free(content);
		return (NULL);
	}
	return (ft_node_ctor(content, type_of_node, ((t_token *)tmp->content)->subshell_level));
}

static void add_operator_node(t_tree *tree, t_node *node)
{
	t_node	*current;

	current = tree->root;
	if (!current)
	{
		tree->root = node;
		return;
	}
	node->left = tree->root;
	tree->root = node;
}

static void	add_non_operator_node(t_tree *tree, t_node *node)
{
	t_node	*current;

	current = tree->root;
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
		add_operator_node(tree, node);
	else
		add_non_operator_node(tree, node);
}

void	ft_printf_tree(t_node *node)
{
	if (!node)
		return ;
	ft_printf_tree(node->left);
	ft_printf("-----------------------\n");
	ft_printf("%s\n", node->content[0]);
	ft_printf("type: %c\n", node->type);
	ft_printf("subshell_level: %d\n", node->subshell_level);
	ft_printf("-----------------------\n");
	ft_printf_tree(node->right);
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
	ft_printf_tree(tree->root);
	return (tree);
}
