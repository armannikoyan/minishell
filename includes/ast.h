#ifndef AST_H
# define AST_H

# include <stdbool.h>

typedef enum node_type
{
	COMMAND_NODE,
	PIPE_NODE,
	AND_NODE,
	OR_NODE,
	REDIRECT_IN_NODE,
	REDIRECT_OUT_NODE,
	REDIRECT_APPEND_NODE,
	HEREDOC_NODE,
	SUBSHELL_NODE,
	ERROR_NODE
}	t_node_type;

typedef enum node_abstract_type {
	CMD_NODE,
	BIN_NODE,
	REDIR_NODE,
	UNDF_NODE,
} t_node_abstract_type;

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

struct	s_subshell
{
	struct s_ast_node	*root;
};

typedef struct s_ast_node
{
	t_node_type	type;
	t_node_abstract_type abstract_type;
	union
	{
		struct s_command		cmd;
		struct s_binary_op		binary;
		struct s_redirection	redir;
		struct s_subshell		subshell;
	}	u_data;
}	t_ast_node;

t_ast_node	*create_cmd_node(t_node_type type, char **argv);
t_ast_node	*create_binary_node(t_node_type type);
t_ast_node	*create_redir_node(t_node_type type, char *filename);
t_ast_node	*create_subshell_node(t_node_type type, t_ast_node *root);

t_ast_node	*ast_build(t_ast_node *new_node, t_ast_node *root);

#endif
