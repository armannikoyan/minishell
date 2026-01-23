#include <dirent.h>
#include <stdbool.h>

#include "../../../libs/libft/libft.h"

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
        // Toggle quote state
        if (!quote_type && (str[i] == '\'' || str[i] == '\"'))
            quote_type = str[i];
        else if (quote_type && str[i] == quote_type)
            quote_type = 0;
        else
        {
            // Copy character and mark its state
            clean[j] = str[i];
            if (quote_type != 0)
                mask[j] = 1; // It's quoted
            else
                mask[j] = 0; // It's not quoted
            j++;
        }
        i++;
    }
    *mask_out = mask;
    return (clean);
}

static bool match_pattern(char *pattern, int *mask, char *string)
{
    // 1. End of strings
    if (*pattern == '\0' && *string == '\0')
        return (true);

    // 2. Wildcard Case: It is '*' AND the mask says it is NOT quoted
    //    We recurse: try matching 0 chars (skip *) OR 1+ chars (consume string)
    if (*pattern == '*' && *mask == 0)
    {
        if (match_pattern(pattern + 1, mask + 1, string))
            return (true);
        if (*string && match_pattern(pattern, mask, string + 1))
            return (true);
        return (false);
    }

    // 3. Literal Match Case (Chars match)
    if (*pattern == *string)
        return (match_pattern(pattern + 1, mask + 1, string + 1));

    return (false);
}

// Requires your t_list functions
static t_list *get_matches(char *raw_token)
{
    DIR             *dir;
    struct dirent   *entry;
    t_list          *matches;
    char            *clean_pattern;
    int             *mask;

    matches = NULL;
    clean_pattern = get_pattern_mask(raw_token, &mask);
    if (!clean_pattern)
        return (NULL);
    dir = opendir(".");
    if (!dir)
        return (free(clean_pattern), free(mask), NULL);
    while ((entry = readdir(dir)) != NULL)
    {
        // Handle hidden files: Only match if pattern specifically starts with '.'
        if (entry->d_name[0] == '.' && clean_pattern[0] != '.')
            continue ;

        if (match_pattern(clean_pattern, mask, entry->d_name))
            ft_lstadd_back(&matches, ft_lstnew(ft_strdup(entry->d_name)));
    }
    closedir(dir);
    free(clean_pattern);
    free(mask);
    return (matches);
}

// Helper to rebuild char ** from list
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

// THE FUNCTION TO CALL IN YOUR PIPELINE
char **expand_wildcards(char **old_argv)
{
    t_list  *final_list;
    t_list  *matches;
    int     i;

    final_list = NULL;
    i = 0;
    while (old_argv[i])
    {
        // Only run wildcard logic if * exists
        if (ft_strchr(old_argv[i], '*'))
        {
            matches = get_matches(old_argv[i]);
            if (matches)
            {
                // Found files: Add them (they are already unquoted filenames)
                ft_lstadd_back(&final_list, matches);
            }
            else
            {
                // No matches: Keep original token (e.g., "*.c")
                // Quote removal will handle it later
                ft_lstadd_back(&final_list, ft_lstnew(ft_strdup(old_argv[i])));
            }
        }
        else
        {
            // No wildcard: Keep original
            ft_lstadd_back(&final_list, ft_lstnew(ft_strdup(old_argv[i])));
        }
        i++;
    }
    // You should free the old_argv array here if needed
    // free_split(old_argv);
    char **result = list_to_argv(final_list);
    ft_lstclear(&final_list, free);
    return (result);
}