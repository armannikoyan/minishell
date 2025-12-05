/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 23:17:52 by anikoyan          #+#    #+#             */
/*   Updated: 2025/12/05 23:44:33 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

typedef enum node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_REDIRECT_IN,
	NODE_REDIRECT_OUT,
	NODE_REDIRECT_APPEND,
	NODE_HEREDOC
}	t_node_type;

struct	s_ast_node;

struct	s_command
{
	char	**argv;
};

struct	s_binary_op
{
	struct ast_node	*left;
	struct ast_node	*right;
};

struct	s_redirection
{
	struct s_ast_node	*child;
	char				*filename;
	int					fd;
};

typedef struct s_ast_node
{
	t_node_type	type;
	union
	{
		struct s_command		cmd;
		struct s_binary_op		binary;
		struct s_redirection	redir;
	}	u_data;
}	t_ast_node;

#endif
