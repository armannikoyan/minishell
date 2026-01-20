#include <stddef.h>

#include "hash_table.h"
#include "../../../libs/libft/libft.h"

size_t   env_var_len(char *str)
{
    size_t	i;

    if (!str)
        return (0);
    i = 0;
    if (ft_isalpha(str[i]) || str[i] == '_')
        ++i;
    while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
        ++i;
    return (i);
}

char *get_env_val(char *str, t_hash_table *ht)
{
    char    *key;
    char    *val;
    size_t  len;

    len = env_var_len(str);
    key = ft_substr(str, 0, len);
    if (!key)
        return (NULL);
    if (ht_get(ht, key) == NULL)
        return (NULL);
    val = ht_get(ht, key)->val;
    free(key);
    return (val);
}