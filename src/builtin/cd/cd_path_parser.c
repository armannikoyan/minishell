#include <stdlib.h>

#include "../../../libs/libft/libft.h"
#include "cd_path_utils.h"
#include "utils.h"

// Frees an array of strings produced by split_env_var.
// Safe to call on fully or partially constructed arrays.
void free_split(char **paths) {
    size_t i = 0;

    if (!paths)
        return;
    while (paths[i]) {
        free(paths[i]);
        i++;
    }
    free(paths);
}

// Counts the number of segments in the colon-separated string.
// Every ':' implies a split, so segments = colons + 1.
static size_t count_segments(const char *str) {
    size_t count = 1;

    while (*str) {
        if (*str == ':')
            count++;
        str++;
    }
    return count;
}

// Creates a new string from the range [start, end).
// If the length is 0 (empty segment), allocates and returns ".".
static char *extract_segment(const char *start, const char *end) {
    char    *segment;
    size_t  len;

    len = end - start;
    if (len == 0) {
        segment = ft_strdup(".");
    } else {
        segment = malloc(len + 1);
        if (segment) {
            ft_memcpy(segment, start, len);
            segment[len] = '\0';
        }
    }
    return segment;
}

// Iterates through the string and fills the allocated array.
// Returns true on success, false on allocation failure.
static bool fill_split_array(char **result, const char *str) {
    const char  *start = str;
    size_t      i = 0;

    while (*str) {
        if (*str == ':') {
            result[i] = extract_segment(start, str);
            if (!result[i])
                return false;
            i++;
            start = str + 1; // Move start to character after colon
        }
        str++;
    }
    // Handle the final segment (trailing path or implicit last path)
    result[i] = extract_segment(start, str);
    if (!result[i])
        return false;
    return true;
}

// Splits a colon-separated environment variable (like CDPATH) into an array.
//
// Behavior:
// - Uses ':' as a delimiter.
// - Empty segments (start, end, or "::") are converted to ".".
// - Returns a NULL-terminated array of strings.
// - Returns NULL if str is NULL, empty, or on malloc failure.
char **split_cd_path(const char *str) {
    char    **result;

    if (!str || !*str)
        return NULL;

    // 1. Allocate the array of pointers
    result = ft_calloc(count_segments(str) + 1, sizeof(char *));
    if (!result) {
        //TODO: make normal error
        print_error("minishell: split_env_var: malloc", false);
        return NULL;
    }

    // 2. Fill the array
    if (!fill_split_array(result, str)) {
        //TODO: make normal error
        print_error("minishell: split_env_var: malloc", false);
        free_split(result);
        return NULL;
    }

    return result;
}
