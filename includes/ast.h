#ifndef AST_H
# define AST_H

# include <stdbool.h>

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
	struct s_ast_node	*left;
	struct s_ast_node	*right;
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

t_ast_node	*create_cmd_node(t_node_type type, char **argv);
t_ast_node	*create_binary_node(t_node_type type);
t_ast_node	*create_redir_node(t_node_type type, char *filename);

t_ast_node	*ast_build(t_ast_node *new_node);

#endif
