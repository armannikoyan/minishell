/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_syntax_tree.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 12:47:00 by anikoyan          #+#    #+#             */
/*   Updated: 2024/12/13 13:09:35 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// static unsigned short	ft_contentlen(t_list **lst)
// {
// 	unsigned short	len;
// 	t_list			*tmp;

// 	len = 0;
// 	tmp = *lst;
// 	if ((t_token *)tmp->content && ((t_token *)tmp->content)->type == 'X')
// 	{
// 		len++;
// 		tmp = tmp->next;
// 		while (tmp && (t_token *)tmp->content
// 			&& ((t_token *)tmp->content)->type != 'O')
// 		{
// 			len++;
// 			tmp = tmp->next;
// 		}
// 		while (tmp && (t_token *)tmp->content
// 			&& (((t_token *)tmp->content)->type == 'O'
// 				|| ((t_token *)tmp->content)->type == 'F'))
// 			tmp = tmp->next;
// 		while (tmp && (t_token *)tmp->content
// 			&& ((t_token *)tmp->content)->type == 'A')
// 		{
// 			len++;
// 			tmp = tmp->next;
// 		}
// 	}
// 	else if ((t_token *)tmp->content && ((t_token *)tmp->content)->type == 'O')
// 	{
// 		if (((t_token *)tmp->content)->type == 'O'
// 			&& ft_strlen(((t_token *)tmp->content)->content) == 2
// 			&& (((t_token *)tmp->content)->content[0] == '>'
// 				|| ((t_token *)tmp->content)->content[0] == '<'))
// 			return (2);
// 		else
// 			return (1);
// 	}
// 	return (len);
// }

// static t_node	*parse_node(t_list **lst)
// {
// 	char			**content;
// 	char			type_of_node;
// 	t_list			*tmp;
// 	unsigned short	i;

// 	if (!lst || !*lst)
// 		exit(EXIT_FAILURE);
// 	type_of_node = 0;
// 	i = 0;
// 	tmp = *lst;
// 	content = (char **)malloc(sizeof(char *) * (ft_contentlen(&tmp) + 1));
// 	if (!content)
// 		exit(EXIT_FAILURE);
// 	if ((t_token *)tmp->content && ((t_token *)tmp->content)->type == 'X')
// 	{
// 		type_of_node = 'X';
// 		while (tmp && (t_token *)tmp->content
// 			&& ((t_token *)tmp->content)->type != 'O')
// 		{
// 			content[i++] = ((t_token *)tmp->content)->content;
// 			tmp = tmp->next;
// 		}
// 		while (tmp && (t_token *)tmp->content
// 			&& (((t_token *)tmp->content)->type == 'O'
// 				|| ((t_token *)tmp->content)->type == 'F'))
// 			tmp = tmp->next;
// 		while (tmp && (t_token *)tmp->content
// 			&& ((t_token *)tmp->content)->type == 'A')
// 		{
// 			content[i++] = ((t_token *)tmp->content)->content;
// 			tmp = tmp->next;
// 		}
// 	}
// 	else if ((t_token *)tmp->content
// 		&& ((t_token *)tmp->content)->type == 'O')
// 	{
// 		type_of_node = 'O';
// 		while (tmp && (t_token *)tmp->content
// 			&& (((t_token *)tmp->content)->type == 'O'
// 				|| ((t_token *)tmp->content)->type == 'F'))
// 		{
// 			content[i++] = ((t_token *)tmp->content)->content;
// 			tmp = tmp->next;
// 		}
// 	}
// 	else
// 	{
// 		free(content);
// 		return (NULL);
// 	}
// 	return (ft_node_ctor(content, type_of_node));
// }

static unsigned short	handle_x_type(t_list **tmp)
{
	unsigned short	len;

	len = 0;
	len++;
	*tmp = (*tmp)->next;
	while (*tmp && ((t_token *)(*tmp)->content)->type != 'O')
	{
		len++;
		*tmp = (*tmp)->next;
	}
	while (*tmp && (((t_token *)(*tmp)->content)->type == 'O'
		|| ((t_token *)(*tmp)->content)->type == 'F'))
		*tmp = (*tmp)->next;
	while (*tmp && ((t_token *)(*tmp)->content)->type == 'A')
	{
		len++;
		*tmp = (*tmp)->next;
	}
	return (len);
}

static unsigned short	handle_o_type(t_list *tmp)
{
	if (ft_strlen(((t_token *)tmp->content)->content) == 2
		&& (((t_token *)tmp->content)->content[0] == '>'
			|| ((t_token *)tmp->content)->content[0] == '<'))
		return (2);
	else
		return (1);
}

static t_node	*free_and_return_null(char **content)
{
	free(content);
	return (NULL);
}

static t_node	*parse_node(t_list **lst)
{
	char			**content;
	t_list			*tmp;

	if (!lst || !*lst)
		exit(EXIT_FAILURE);
	tmp = *lst;
	content = initialize_content_array(&tmp);
	if (((t_token *)tmp->content)->type == 'X')
		return (handle_type_node(content, &tmp, 'X'));
	else if (((t_token *)tmp->content)->type == 'O')
		return (handle_type_node(content, &tmp, 'O'));
	else
		return (free_and_return_null(content));
}

// static void	add_to_tree(t_tree *tree, t_node *node)
// {
// 	t_node	*current;

// 	if (!tree || !node)
// 		return ;
// 	if (!tree->root)
// 	{
// 		tree->root = node;
// 		return ;
// 	}
// 	current = tree->root;
// 	if (node->type == 'O')
// 	{
// 		if (operator_has_higher_precedence(node, current))
// 		{
// 			node->left = current;
// 			tree->root = node;
// 		}
// 		else
// 		{
// 			while (current->right)
// 				current = current->right;
// 			current->right = node;
// 		}
// 	}
// 	else
// 	{
// 		while (current->right)
// 			current = current->right;
// 		if (!current->left)
// 			current->left = node;
// 		else
// 			current->right = node;
// 	}
// }

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
