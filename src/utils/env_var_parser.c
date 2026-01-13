#include <stdlib.h>

#include "utils.h"
#include "../../libs/libft/libft.h"

// Counts the number of path segments in a colon-separated string.
//
// Assumes the string is non-NULL and non-empty.
// Each ':' increases the number of segments by one.
// Used to preallocate an array of pointers for the split result.
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
//
// Allocates a new null-terminated string containing exactly `len` characters from `start`.
// Returns NULL on allocation failure.
static char *substr_dup(const char *start, size_t len) {
    char *s = malloc(len + 1);
    if (!s)
        return NULL;

    ft_memcpy(s, start, len);
    s[len] = '\0';
    return s;
}

// Frees an array of strings produced by split_env_var.
//
// Assumes the array is NULL-terminated.
// Safe to call only on fully or partially constructed split results.
void free_split(char **cd_path) {
    size_t i = 0;

    while (cd_path[i])
        free(cd_path[i++]);
    free(cd_path);
}

// Performs basic validation and initialization for splitting a colon-separated environment variable.
//
// Semantics:
// - If `str` is NULL, the variable is considered unset → returns NULL.
// - If `str` is an empty string, the variable is considered disabled → returns NULL.
// - Rejects values starting or ending with ':' (empty path segment).
//
// On success, allocates and returns an array of string pointers sized
// to hold all path segments plus a terminating NULL.
// The returned array is zero-initialized.
//
// Returns NULL on validation failure or allocation error.
static char **basic_checks(const char *str) {
    char **result;

    if (str == NULL || str[0] == '\0')
        return NULL;

    if (str[0] == ':' || str[ft_strlen(str) - 1] == ':') {
        // TODO: make normal error
        print_error("cd: split_env_var: basic_checks: empty path\n", true);
        return NULL;
    }

    result = ft_calloc((count_parts(str) + 1), sizeof(char *));
    // TODO: make normal error
    if (!result)
        return (print_error("cd: split_env_var: malloc", false), NULL);

    return result;
}


// Splits a colon-separated environment variable into an array of paths.
//
// Behavior:
// - Uses ':' as a delimiter.
// - Rejects empty segments (e.g. "::", ":path", "path:").
// - Returns a NULL-terminated array of newly allocated strings.
// - If the variable is unset or empty, returns NULL.
//
// On error, prints an error message, frees any allocated memory, and returns NULL.
char **split_env_var(const char *str) {
    char **result;
    const char *start;
    size_t i = 0;

    result = basic_checks(str);
    if (result == NULL)
        return NULL;

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
    result[i] = substr_dup(start, str - start);
    return result;
}
