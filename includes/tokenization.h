#ifndef TOKENIZATION_H
# define TOKENIZATION_H

#include <stdbool.h>

#include "ast.h"

bool	is_redir(char *str);
bool	is_operator(char *str);
t_node_type	get_node_type(char *input);
size_t	get_operator_len(t_node_type type);

t_ast_node	*tokenize(char *input);

#endif
