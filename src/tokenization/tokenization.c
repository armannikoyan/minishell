#include <stddef.h>
#include <stdlib.h>

#include "../../includes/tokenization.h"

#include "utils.h"
#include "../../libs/libft/libft.h"

static t_ast_node	*construct_subshell_node(char *input, size_t *i, bool *is_iter_skippable)
{
	char		*sub_str;
	t_ast_node *sub_tree;

	sub_str = extract_subshell_content(input, i);
	if (!sub_str)
		return (NULL);
	if (ft_strlen(sub_str) == 0)
	{
		print_error("minishell: parsing error near unexpected token `)'\n", true);
		free(sub_str);
		return (NULL);
	}
	sub_tree = tokenize(sub_str);
	free(sub_str);
	if (!sub_tree)
	{
		*is_iter_skippable = true;
		return (NULL);
	}
	return (create_subshell_node(SUBSHELL_NODE, sub_tree));
}

static t_ast_node	*construct_node(char *input, size_t *i, t_node_type type, bool *is_iter_skippable)
{
	t_ast_node	*node;

	node = NULL;
	if (type == COMMAND_NODE)
		node = create_cmd_node(COMMAND_NODE, get_argv(input, i));
	else if (type >= PIPE_NODE && type <= HEREDOC_NODE)
	{
		*i += get_operator_len(type);
		if (type >= REDIRECT_IN_NODE)
			node = create_redir_node(type, substr_next(input, i));
		else
			node = create_binary_node(type);
	}
	else if (type == SUBSHELL_NODE)
		node = construct_subshell_node(input, i, is_iter_skippable);
	else if (type == ERROR_NODE)
	{
		print_error("minishell: parsing error near unexpected token `)'\n", true);
		return (NULL);
	}
	return (node);
}

t_ast_node	*tokenize(char *input)
{
	t_ast_node	*node;
	size_t	i;
    t_ast_node *head_node;
	bool		is_iter_skippable;

	i = 0;
	node = NULL;
    head_node = NULL;
	while (input[i])
	{
		while (input[i] == ' ' || input[i] == '\t')
			++i;
		if (!input[i])
			break ;
		is_iter_skippable = false;
		node = construct_node(input, &i, get_node_type(&(input[i])), &is_iter_skippable);
		if (is_iter_skippable)
			continue ;
		if (node)
		{
			head_node = ast_build(node, head_node);
			// print_ast_info(head_node, node);
		}
		else
		{
			// TODO: Clear the tree
			return (NULL);
		}
	}
	return (head_node);
}
