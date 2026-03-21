#ifndef TOKENIZATION_H
# define TOKENIZATION_H

# include <stdbool.h>

# include "ast.h"

bool is_redir(const char *str);

bool is_operator(const char *str);

t_node_type get_node_type(char *input);

size_t get_operator_len(t_node_type type);

void set_quote_char(char c, char *quote_char);

char *extract_subshell_content(const char *input, size_t *i, int *errnum);

char *substr_next(const char *input, size_t *pos);

char **get_argv(char *input, size_t *pos);

t_ast_node *tokenize(char *input, int *errnum);

#endif
