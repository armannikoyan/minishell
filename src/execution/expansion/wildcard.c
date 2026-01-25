#include <dirent.h>
#include <stdbool.h>

#include "execution.h"
#include "../../../libs/libft/libft.h"

// [Unchanged] Generates mask: 1 = quoted, 0 = unquoted
static char *get_pattern_mask(char *str, int **mask_out)
{
    char    *clean;
    int     *mask;
    int     i;
    int     j;
    char    quote_type;

    clean = ft_calloc(ft_strlen(str) + 1, sizeof(char));
    mask = ft_calloc(ft_strlen(str) + 1, sizeof(int));
    if (!clean || !mask)
        return (free(clean), free(mask), NULL);
    i = 0;
    j = 0;
    quote_type = 0;
    while (str[i])
    {
        if (!quote_type && (str[i] == '\'' || str[i] == '\"'))
            quote_type = str[i];
        else if (quote_type && str[i] == quote_type)
            quote_type = 0;
        else
        {
            clean[j] = str[i];
            if (quote_type != 0)
                mask[j] = 1;
            else
                mask[j] = 0;
            j++;
        }
        i++;
    }
    *mask_out = mask;
    return (clean);
}

// [Unchanged] Recursively checks pattern against string using the mask
static bool match_pattern(char *pattern, int *mask, char *string)
{
    if (*pattern == '\0' && *string == '\0')
        return (true);

    if (*pattern == '*' && *mask == 0)
    {
        if (match_pattern(pattern + 1, mask + 1, string))
            return (true);
        if (*string && match_pattern(pattern, mask, string + 1))
            return (true);
        return (false);
    }

    if (*pattern == *string)
        return (match_pattern(pattern + 1, mask + 1, string + 1));

    return (false);
}

static void sort_ascii(t_list *lst)
{
    t_list  *head;
    t_list  *node;
    char    *tmp_content;

    if (!lst)
        return ;
    head = lst;
    while (head)
    {
        node = head->next;
        while (node)
        {
            if (ft_strcmp(head->content, node->content) > 0)
            {
                tmp_content = head->content;
                head->content = node->content;
                node->content = tmp_content;
            }
            node = node->next;
        }
        head = head->next;
    }
}

static t_list *get_matches(char *raw_token)
{
    DIR             *dir;
    struct dirent   *entry;
    t_list          *matches;
    char            *clean_pattern;
    int             *mask;

    matches = NULL;
    // Pass RAW token to preserve quote info in mask
    clean_pattern = get_pattern_mask(raw_token, &mask);
    if (!clean_pattern)
        return (NULL);
    dir = opendir(".");
    if (!dir)
        return (free(clean_pattern), free(mask), NULL);
    while ((entry = readdir(dir)) != NULL)
    {
        // [FIXED] 1. Always skip "." and ".." regardless of pattern
        if (ft_strcmp(entry->d_name, ".") == 0 || ft_strcmp(entry->d_name, "..") == 0)
            continue ;

        // 2. Handle hidden files (only match if pattern starts with literal .)
        if (entry->d_name[0] == '.' && clean_pattern[0] != '.')
            continue ;

        // 3. Check pattern match
        if (match_pattern(clean_pattern, mask, entry->d_name))
            ft_lstadd_back(&matches, ft_lstnew(ft_strdup(entry->d_name)));
    }
    closedir(dir);
    free(clean_pattern);
    free(mask);
    sort_ascii(matches);
    return (matches);
}

static char **list_to_argv(t_list *lst)
{
    char    **new_argv;
    int     size;
    int     i;

    size = ft_lstsize(lst);
    new_argv = malloc(sizeof(char *) * (size + 1));
    if (!new_argv)
        return (NULL);
    i = 0;
    while (lst)
    {
        new_argv[i++] = ft_strdup(lst->content);
        lst = lst->next;
    }
    new_argv[i] = NULL;
    return (new_argv);
}

char **expand_wildcards(char **old_argv)
{
    t_list  *final_list;
    t_list  *matches;
    char    *unquoted_str;
    int     i;

    final_list = NULL;
    i = 0;
    while (old_argv[i])
    {
        if (ft_strchr(old_argv[i], '*'))
        {
            // Pass raw string (e.g. `"."*`) so quotes are analyzed
            matches = get_matches(old_argv[i]);
            if (matches)
            {
                ft_lstadd_back(&final_list, matches);
            }
            else
            {
                // No matches: Treat as literal, remove quotes
                unquoted_str = remove_quotes(old_argv[i]);
                ft_lstadd_back(&final_list, ft_lstnew(unquoted_str));
            }
        }
        else
        {
            // No wildcard: Just remove quotes
            unquoted_str = remove_quotes(old_argv[i]);
            ft_lstadd_back(&final_list, ft_lstnew(unquoted_str));
        }
        i++;
    }
    char **result = list_to_argv(final_list);
    ft_lstclear(&final_list, free);
    return (result);
}