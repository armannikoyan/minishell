#include <unistd.h>
#include <stdio.h>

#include "expansion.h"
#include "hash_table.h"
#include "tokenization.h"
#include "utils.h"
#include "../../../libs/libft/libft.h"

static size_t	get_var_len(char *str, size_t *i, t_hash_table *ht, int errnum)
{
    size_t	len;
    char	*tmp;
    char	*val;

    len = 0;
    if (str[*i] == '?')
    {
        tmp = ft_itoa(errnum);
        if (tmp)
            len = ft_strlen(tmp);
        free(tmp);
        ++(*i);
    }
    else
    {
        val = get_env_val(&str[*i], ht);
        if (val)
            len = ft_strlen(val);
        *i += env_var_len(&str[*i]);
    }
    return (len);
}

static size_t	get_expanded_strlen(char *str, t_hash_table *ht, int errnum)
{
    size_t	i;
    size_t	len;
    char	quote_char;

    i = 0;
    len = 0;
    quote_char = 0;
    while (str[i])
    {
        set_quote_char(str[i], &quote_char);
        if (str[i] == '$' && quote_char != '\'')
        {
            // Check if the NEXT character is valid for expansion
            if (str[i + 1] == '?' || ft_isalpha(str[i + 1]) || str[i + 1] == '_')
            {
                ++i;
                len += get_var_len(str, &i, ht, errnum);
            }
            else
            {
                // Treat $ as a literal char
                ++len;
                ++i;
            }
        }
        else
        {
            ++len;
            ++i;
        }
    }
    return (len);
}

static size_t	copy_var_val(char *dst, char *str, size_t *i, t_hash_table *ht, int errnum)
{
    size_t	len;
    char	*tmp;
    char	*val;

    len = 0;
    if (str[*i] == '?')
    {
        tmp = ft_itoa(errnum);
        if (tmp)
        {
            len = ft_strlen(tmp);
            ft_memcpy(dst, tmp, len);
            free(tmp);
        }
        ++(*i);
    }
    else
    {
        val = get_env_val(&str[*i], ht);
        if (val)
        {
            len = ft_strlen(val);
            ft_memcpy(dst, val, len);
        }
        *i += env_var_len(&str[*i]);
    }
    return (len);
}

static void	fill_exp_str(char *new_str, char *str, t_hash_table *ht, int errnum)
{
    size_t	i;
    size_t	j;
    char	quote_char;

    i = 0;
    j = 0;
    quote_char = 0;
    while (str[i])
    {
        set_quote_char(str[i], &quote_char);
        if (str[i] == '$' && quote_char != '\'')
        {
            // Check if the NEXT character is valid for expansion
            if (str[i + 1] == '?' || ft_isalpha(str[i + 1]) || str[i + 1] == '_')
            {
                ++i;
                j += copy_var_val(new_str + j, str, &i, ht, errnum);
            }
            else
            {
                // Copy $ as a literal char
                new_str[j++] = str[i++];
            }
        }
        else
            new_str[j++] = str[i++];
    }
    new_str[j] = '\0';
}

char	*expand_dollar_sign(char *str, t_hash_table *ht, int errnum)
{
    char    *new_str;
    size_t	len;

    len = get_expanded_strlen(str, ht, errnum);
    if (len == 0)
        return (ft_strdup(""));
    new_str = (char *)ft_calloc(sizeof(char), len + 1);
    if (!new_str) {
        print_error("minishell: malloc", false);
        free(str);
        return (NULL);
    }
    fill_exp_str(new_str, str, ht, errnum);
    return (new_str);
}