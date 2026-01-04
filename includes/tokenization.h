#ifndef TOKENIZATION_H
# define TOKENIZATION_H

#include <stdbool.h>

#include "ast.h"

bool	is_redir(char *str);
bool	is_operator(char *str);

t_ast_node	*tokenize(char *input);

#endif
