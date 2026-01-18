#include <stddef.h>

#include "../../includes/tokenization.h"
#include "../../includes/utils.h"
#include "../../libs/libft/libft.h"

static void get_subshell_nesting_info(char *input, size_t *i, int *nested_level) {
    char	quote_char;

    quote_char = 0;
    while (input[*i])
    {
        set_quote_char(input[*i], &quote_char);
        if (quote_char == 0)
        {
            if (input[*i] == '(')
                ++(*nested_level);
            else if (input[*i] == ')')
                --(*nested_level);
            if (*nested_level == 0)
                break ;
        }
        ++(*i);
    }
}

char	*extract_subshell_content(char *input, size_t *i)
{
    size_t	start;
    char	*tmp;
    int		nested_level;

    start = ++(*i);
    nested_level = 1;
    get_subshell_nesting_info(input, i, &nested_level);
    if (nested_level != 0)
    {
        print_error("minishell: parsing error near unexpected token `('\n", true);
        return (NULL);
    }
    tmp = ft_substr(input, start, *i - start);
    if (input[*i] == ')')
        ++(*i);
    return (tmp);
}

char	*substr_next(char *input, size_t *i)
{
    size_t		start;
    char	quote_char;

    while (input[*i] == ' ' || input[*i] == '\t')
        ++(*i);
    start = *i;
    quote_char = 0;
    while (input[*i])
    {
        set_quote_char(input[*i], &quote_char);
        if (quote_char == 0 && (input[*i] == ' ' || input[*i] == '\t' ||
            is_operator(&(input[*i])) || is_redir(&(input[*i]))))
            break ;
        ++(*i);
    }
    return (ft_substr(input, start, *i - start));
}

static size_t   get_argv_len(char *input, size_t *i)
{
    size_t	j;
    size_t	len;
    char	quote_char;

    j = *i;
    len = 0;
    while (input[j] && !is_operator(&(input[j])) && !is_redir(&(input[j])))
    {
        while (input[j] == ' ' || input[j] == '\t')
            j++;
        if (input[j] && !is_operator(&(input[j])) && !is_redir(&(input[j])))
        {
            len++;
            quote_char = 0;
            while (input[j])
            {
                set_quote_char(input[j], &quote_char);
                if (quote_char == 0 && (input[j] == ' ' || input[j] == '\t' ||
                    is_operator(&(input[j])) || is_redir(&(input[j]))))
                    break ;
                j++;
            }
            if (quote_char != 0) {
                print_error("minishell: parsing error near unexpected token `", true);
                write(2, &quote_char, 1);
                write(2, "'\n", 2);
                return (0);
            }
        }
    }
    return (len);
}

char	**get_argv(char *input, size_t *i)
{
    char	**argv;
    size_t	len;
    size_t	j;

    len = get_argv_len(input, i);
    if (len == 0)
        return (NULL);
    argv = (char **)malloc(sizeof(char *) * (len + 1));
    if (!argv) {
        print_error("minishell: malloc", false);
        return (NULL);
    }
    j = 0;
    while (j < len)
    {
        argv[j] = substr_next(input, i);
        if (!argv[j])
        {
            free_split(argv);
            return (NULL);
        }
        ++j;
    }
    argv[len] = NULL;
    return (argv);
}