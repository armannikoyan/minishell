#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "utils.h"
#include "../../libs/libft/libft.h"

// Counts the number of path segments in a colon-separated string.
// Each ':' increases the number of segments by one.
// Used to preallocate an array of pointers for split result.
static size_t count_parts(const char *str) {
    size_t count = 1;

    while (*str) {
        if (*str == ':')
            count++;
        str++;
    }
    return count;
}

// Duplicates a substring defined by [start, start + len).
// Allocates a new null-terminated string.
// Returns NULL on allocation failure.
static char *substr_dup(const char *start, size_t len) {
    char *s = malloc(len + 1);
    if (!s)
        return NULL;

    ft_memcpy(s, start, len);
    s[len] = '\0';
    return s;
}

// Frees an array of strings representing a split of environment variable.
// Assumes the array is NULL-terminated.
void free_split(char **cd_path) {
    size_t i = 0;

    while (cd_path[i])
        free(cd_path[i++]);
    free(cd_path);
}

// Performs basic validation and early handling of the environment variable string.
//
// - If str is NULL, returns an array with a single element ".".
// - Rejects paths starting or ending with ':' (empty segment).
// - Allocates the result array for further parsing.
//
// Sets *is_done to true if parsing should continue in split_env_var.
// Returns NULL on error.
static char **basic_checks(const char *str, bool *is_done) {
    char **result;

    if (str == NULL) {
        result = (char **) ft_calloc(2, sizeof(char *));
        if (result == NULL) {
            // TODO: make normal error
            print_error("cd: split_env_var: basic_checks: malloc", false);
            return NULL;
        }
        result[0] = ft_strdup(".");
        if (result[0] == NULL) {
            // TODO: make normal error
            print_error("cd: split_env_var: basic_checks: strdup: malloc", false);
            free_split(result);
            return NULL;
        }
        return result;
    }
    if (str[0] == ':' || str[strlen(str) - 1] == ':') {
        // TODO: make normal error
        print_error("cd: split_env_var: basic_checks: empty path\n", true);
        return NULL;
    }
    result = malloc(sizeof(char *) * (count_parts(str) + 1));
    if (!result) {
        // TODO: make normal error
        return (print_error("cd: split_env_var: malloc", false), NULL);
    }
    *is_done = true;
    return result;
}

// Splits a colon-separated environment variable string into an array of paths.
//
// - Uses ':' as a delimiter.
// - Rejects empty segments (e.g. "::" or ":path").
// - Returns a NULL-terminated array of strings.
//
// On error, prints an error message and returns NULL.
char **split_env_var(const char *str) {
    char **result;
    const char *start;
    bool is_done;
    size_t i = 0;

    is_done = false;
    result = basic_checks(str, &is_done);
    if (is_done == false)
        return (result);

    start = str;
    while (*str) {
        if (*str == ':') {
            if (str == start) {
                // TODO: make normal error
                print_error("cd: split_env_var: basic_checks: empty path\n", true);
                return (free_split(result), NULL);
            }
            result[i++] = substr_dup(start, str - start);
            start = str + 1;
        }
        str++;
    }
    result[i++] = substr_dup(start, str - start);
    result[i] = NULL;
    return result;
}
