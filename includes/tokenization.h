#ifndef TOKENIZATION_H
# define TOKENIZATION_H

#include <stdbool.h>

#include "ast.h"

bool	is_redir(char *str);
bool	is_operator(char *str);
t_node_type	get_node_type(char *input);
size_t	get_operator_len(t_node_type type);
void	set_quote_char(char c, char *quote_char);

char	*extract_subshell_content(char *input, size_t *i);
char	*substr_next(char *input, size_t *i);
char	**get_argv(char *input, size_t *i);

t_ast_node	*tokenize(char *input, int *errnum);

#endif
