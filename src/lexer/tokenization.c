/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 19:52:06 by anikoyan          #+#    #+#             */
/*   Updated: 2025/12/07 21:57:43 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../../libs/libft/libft.h"
#include "../../includes/ast.h"

static bool	is_redir(char *str)
{
	if (!ft_strncmp(str, "<<", 2) || *str == '<'
		|| !ft_strncmp(str, ">>", 2) || *str == '>')
		return (true);
	return (false);
}

static bool	is_operator(char *str)
{
	if (!ft_strncmp(str, "||", 2) || *str == '|'
		|| !ft_strncmp(str, "&&", 2))
		return (true);
	return (false);
}

static char	*substr_next(char *input, size_t *i)
{
	char		*tmp;
	size_t		len;
	size_t		start;
	size_t		j;

	while (input[*i] == ' ')
		++(*i);
	start = *i;
	while (input[*i] && input[*i] != ' ' && !is_operator(&(input[*i])))
		++(*i);
	len = *i - start;
	tmp = (char *)malloc(sizeof(char) * (len + 1));
	j = 0;
	while (j < len)
	{
		tmp[j] = input[start + j];
		++j;
	}
	tmp[j] = '\0';
	return (tmp);
}

static char	**get_argv(char *input, size_t *i)
{
	char	**argv;
	size_t	len;
	size_t	j;

	j = *i;
	len = 0;
	while (input[j] && !is_operator(&(input[j])))
	{
		while (input[j] == ' ')
			++j;
		if (input[j] && !is_operator(&(input[j])))
		{
			++len;
			while (input[j] && input[j] != ' ' && !is_operator(&(input[j])))
				++j;
		}
	}
	if (len == 0)
		return (NULL);
	argv = (char **)malloc(sizeof(char *) * (len + 1));
	j = 0;
	while (j < len)
		argv[j++] = substr_next(input, i);
	argv[len] = NULL;
	return (argv);
}

// TODO: Identify TYPE and insert nodes into the tree
t_ast_node	*lexer(char *input)
{
	t_ast_node	*node;
	size_t	i;

	i = 0;
	while (input[i])
	{
		if (!is_redir(&(input[i])) && !is_operator(&(input[i])))
			node = create_cmd_node(NODE_COMMAND, get_argv(input, &i));
		else if (is_redir(&(input[i])))
		{
			while (input[i] != ' ')
				++i;
			node = create_redir_node(TYPE, substr_next(input, &i), -1);
		}
		else if (is_operator(&(input[i])))
			node = create_binary_node(TYPE);
		if (node)
			ast_build(node);
		++i;
	}
	return (NULL);
}
