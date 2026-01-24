#include <stddef.h>
#include "../../../libs/libft/libft.h"
#include "cd_path_utils.h"
#include "path_utils.h"

// Append a directory component into `res` using only ft_memcpy and literal '/'.
//
// Arguments:
// - res: writable result buffer (single allocation, size >= original path length + 1).
// - res_len: pointer to current length of data in `res` (bytes used, without NUL).
// - dir: pointer to the directory component inside original path (not NUL-terminated).
// - dir_len: length of the component in bytes.
//
// Algorithm:
// 1) If res contains more than root "/", insert one '/' separator.
// 2) Copy dir_len bytes from `dir` into `res + *res_len` using ft_memcpy.
// 3) Update *res_len and write terminating '\0'.
// 4) Validate resulting path with check_access().
//
// Returns:
// - 0 on success
// - 1 on validation failure (caller must free `res`)
static int append_and_validate(char *res, size_t *res_len, const char *dir, size_t dir_len) {
    int c;

    if (*res_len > 1)
        res[(*res_len)++] = '/';
    ft_memcpy(res + *res_len, dir, dir_len);
    *res_len += dir_len;
    res[*res_len] = '\0';

    c = check_access(res);
    if (!(c & EXISTS_BIT) || (c & STAT_FAIL_BIT) || !(c & DIR_BIT) || !(c & EXEC_BIT))
        return 1;
    return 0;
}

// Remove the last directory component from `res` and validate the result.
//
// Algorithm:
// 1) Mutate string in place via remove_last_dir(res).
// 2) Update *res_len to match new string length (CRITICAL FIX).
// 3) Validate resulting path with check_access().
//
// Returns:
// - 0 on success
// - 1 on validation failure (caller must free `res`)
static int remove_and_validate(char *res, size_t *res_len) {
    int c;

    remove_last_dir(res);
    *res_len = ft_strlen(res); // Update length to prevent heap corruption on next append

    c = check_access(res);
    if (!(c & EXISTS_BIT) || (c & STAT_FAIL_BIT) || !(c & DIR_BIT) || !(c & EXEC_BIT))
        return 1;
    return 0;
}

// Iterate over path components and apply them to `res`.
//
// Arguments:
// - res: result buffer (single allocation).
// - res_len: pointer to current length of `res`.
// - p: pointer into original absolute path where processing starts.
//
// Processing rules:
// - multiple '/' are ignored
// - "." components are ignored
// - ".." components call remove_and_validate
// - normal components call append_and_validate
//
// Returns:
// - 0 on success
// - 1 on any validation failure
static int process_components(char *res, size_t *res_len, const char *p) {
    const char *dir_start;
    size_t dir_len;

    while (*p) {
        while (*p == '/')
            p++;
        if (!*p)
            break;

        dir_start = p;
        while (*p && *p != '/')
            p++;

        dir_len = (size_t) (p - dir_start);

        // Ignore "."
        if (dir_len == 1 && dir_start[0] == '.')
            continue;

        // Handle ".."
        if (dir_len == 2 && dir_start[0] == '.' && dir_start[1] == '.') {
            if (remove_and_validate(res, res_len))
                return 1;
            continue;
        }

        // Handle normal directory
        if (append_and_validate(res, res_len, dir_start, dir_len))
            return 1;
    }
    return 0;
}

// Phase 1 of normalization: copy components until the first "..".
//
// Behavior:
// - skips repeated '/'
// - ignores "."
// - stops when first ".." is encountered
// - appends only normal components into `res`
// - validates after each append
//
// Returns:
// - pointer `p` to position in original path where first ".." starts
// - NULL on validation failure (caller must free `res`)
static const char *copy_until_dotdot(char *res, size_t *res_len, const char *p) {
    const char *start;

    while (*p)
        {
        while (*p == '/')
            p++;
        if (*p == '\0')
            break;

        start = p;
        while (*p && *p != '/')
            p++;

        if ((p - start) == 1 && start[0] == '.')
            continue;

        if ((p - start) == 2 && start[0] == '.' && start[1] == '.')
            {
            p -= 2;
            break; // Stop and return pointer to ".."
        }

        if (append_and_validate(res, res_len, start, (p - start)))
            return NULL;
    }
    return p;
}

// Normalize absolute `path` and resolve "..".
//
// Guarantees:
// - single allocation (size == strlen(path) + 1)
// - only in-place modification of result buffer
// - root "/" cannot be removed
//
// Algorithm:
// 1) Allocate result buffer and initialize it as "/".
// 2) Phase 1: copy components until first ".." (copy_until_dotdot).
// 3) If no ".." exists -> return result.
// 4) Phase 2: iteratively process remaining components.
//
// Returns:
// - normalized absolute path on success
// - NULL on any validation or allocation failure
char *normalize_and_resolve_path(const char *path) {
    char *res;
    const char *p;
    size_t res_len;

    if (!path)
        return NULL;
    res = (char *) ft_calloc(ft_strlen(path) + 1, sizeof(char));
    if (res == NULL)
        return NULL;

    res[0] = '/';
    res_len = 1;

    // Phase 1: Fast copy until we hit complexity ("..")
    p = copy_until_dotdot(res, &res_len, path);
    if (p == NULL) {
        free(res);
        return NULL;
    }

    // If we reached the end without "..", we are done
    if (*p == '\0')
        return res;

    // Phase 2: Handle the rest, including ".." logic
    if (process_components(res, &res_len, p)) {
        free(res);
        return NULL;
    }

    return res;
}
