#include <stddef.h>

#include "../../includes/tokenization.h"
#include "../../includes/utils.h"
#include "../../libs/libft/libft.h"

/*
** Helper to skip over a redirection operator and its filename.
** Updates the index 'j' to point after the filename.
*/
static void skip_redirection(char *input, size_t *j)
{
    char quote_char;

    // 1. Skip the operator (<<, >>, <, >)
    if (is_redir(&(input[*j + 1])) && input[*j] == input[*j + 1])
        *j += 2;
    else
        (*j)++;

    // 2. Skip spaces between operator and filename
    while (input[*j] == ' ' || input[*j] == '\t')
        (*j)++;

    // 3. Skip the filename (treat it as a quoted word)
    quote_char = 0;
    while (input[*j])
    {
        set_quote_char(input[*j], &quote_char);
        // Stop if unquoted space, operator, or redir is found
        if (quote_char == 0 && (input[*j] == ' ' || input[*j] == '\t' ||
            is_operator(&(input[*j])) || is_redir(&(input[*j]))))
            break ;
        (*j)++;
    }
}

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
    // Loop until end of string or a Logic/Pipe operator (&&, ||, |)
    // Removed !is_redir check to allow skipping redirections
    while (input[j] && !is_operator(&(input[j])))
    {
        while (input[j] == ' ' || input[j] == '\t')
            j++;
        if (!input[j] || is_operator(&(input[j])))
            break ;

        if (is_redir(&(input[j])))
        {
            skip_redirection(input, &j);
            continue ;
        }

        // It is an argument; count it
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
    return (len);
}

char	**get_argv(char *input, size_t *i)
{
    char	**argv;
    size_t	len;
    size_t	cur;
    size_t  start;
    size_t	idx;
    bool    skipped_redir;
    char    quote_char;

    len = get_argv_len(input, i);
    if (len == 0)
        return (NULL);
    argv = (char **)malloc(sizeof(char *) * (len + 1));
    if (!argv) {
        print_error("minishell: malloc", false);
        return (NULL);
    }

    idx = 0;
    cur = *i;
    skipped_redir = false;

    while (idx < len)
    {
        while (input[cur] == ' ' || input[cur] == '\t')
            cur++;

        if (is_redir(&(input[cur])))
        {
            skip_redirection(input, &cur);
            skipped_redir = true;
            continue;
        }

        // Extract Argument
        start = cur;
        quote_char = 0;
        while (input[cur])
        {
            set_quote_char(input[cur], &quote_char);
            if (quote_char == 0 && (input[cur] == ' ' || input[cur] == '\t' ||
                is_operator(&(input[cur])) || is_redir(&(input[cur]))))
                break ;
            cur++;
        }

        argv[idx] = ft_substr(input, start, cur - start);
        if (!argv[idx])
        {
            free_split(argv);
            return (NULL);
        }

        // CRITICAL: If we haven't skipped a redir yet, this is a contiguous
        // argument (like command name or first arg), so we safely advance main pointer *i.
        // If we HAVE skipped a redir, this argument is "floating" after a redir.
        // We must blank it out so the main tokenizer loop ignores it later.
        if (!skipped_redir)
            *i = cur;
        else
            ft_memset(input + start, ' ', cur - start);

        idx++;
    }
    argv[len] = NULL;
    return (argv);
}