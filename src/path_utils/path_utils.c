#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "path_utils.h"
#include "utils.h"

// Concatenates two path components into a single path.
// Automatically inserts a '/' between components if needed.
// Returns a newly allocated string which must be freed by the caller.
char *concat_path(const char *p1, const char *p2) {
  if (!p1 || !p2)
    return NULL;

  const size_t len1 = strlen(p1);
  const size_t len2 = strlen(p2);
  const bool needs_slash = (len1 > 0 && p1[len1 - 1] != '/');

  char *full = malloc(len1 + len2 + needs_slash + 1);
  if (!full) {
    print_error("minishell: concat_path: malloc", false);
    return NULL;
  }

  memcpy(full, p1, len1);
  if (needs_slash) {
    full[len1] = '/';
    memcpy(full + len1 + 1, p2, len2 + 1);
  } else
    memcpy(full + len1, p2, len2 + 1);

  return full;
}

// Removes the last directory component from an absolute path.
// The string is modified in place.
// If the path is "/", it is left unchanged.
char *remove_last_dir(char *path) {
  if (!path || path[0] != '/')
    return path;

  if (path[1] == '\0')
    return path;

  char *last_slash = strrchr(path, '/');

  if (last_slash) {
    if (last_slash == path)
      path[1] = '\0';
    else
      *last_slash = '\0';
  }

  return path;
}

// Expands the '~' symbol using the HOME variable from the environment hash
// table. Supports only '~' and '~/...' forms. Expansion of '~user' is
// intentionally not supported due to project restrictions. Returns a newly
// allocated string or NULL on error.
char *expand_tilda(const char *path, t_hash_table *ht) {
  if (!path || path[0] != '~')
    return NULL;

  const char *rest = NULL;
  if (path[1] == '/')
    rest = path + 2;
  else if (path[1] == '\0')
    rest = "";
  else
    return NULL;

  t_entry *entry = ht_get(ht, "HOME");
  if (!entry) {
    print_error("minishell: expand_tilda: HOME not set\n", true);
    return NULL;
  }

  const char *home = entry->val ? entry->val : "";
  return concat_path(home, rest);
}

// Checks filesystem properties of a given path and returns the result
// as a bitmask.
//
// The function verifies existence, determines file type (directory or
// regular file), and checks access permissions (read, write, execute).
//
// Returned value is a bitwise OR of FS_* flags defined in utils.h.
// The caller is responsible for interpreting the result.
int check_access(const char *path) {
  struct stat st;
  int result = 0;

  if (access(path, F_OK) != 0)
    return result;

  result |= EXISTS_BIT;

  if (stat(path, &st) != 0) {
    result |= STAT_FAIL_BIT;
    return result;
  }

  if (S_ISDIR(st.st_mode))
    result |= DIR_BIT;
  else if (S_ISREG(st.st_mode))
    result |= FILE_BIT;

  if (access(path, R_OK) == 0)
    result |= READ_BIT;
  if (access(path, W_OK) == 0)
    result |= WRITE_BIT;
  if (access(path, X_OK) == 0)
    result |= EXEC_BIT;

  return result;
}
