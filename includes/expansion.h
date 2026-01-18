#ifndef EXPANSION_H
# define EXPANSION_H

# include "hash_table.h"

size_t   env_var_len(char *str);
char *get_env_val(char *str, t_hash_table *ht);

char    *expand_dollar_sign(char *str, t_hash_table *ht);

#endif