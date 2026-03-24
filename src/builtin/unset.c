#include <ctype.h>
#include <stddef.h>

#include "error_codes.h"
#include "hash_table.h"

static bool is_ev_correct(const char *str) {
  if (!str || !(isalpha(str[0]) || str[0] == '_'))
    return false;

  size_t len = 1;
  while (str[len] && (isalnum(str[len]) || str[len] == '_'))
    ++len;

  return true;
}

int ft_unset(const int argc, char **argv, t_hash_table *ht,
             const int errnum __attribute__((unused))) {
  if (argc == 1)
    return 0;

  bool is_err = false;
  for (int i = 0; argv[i]; ++i) {
    if (is_ev_correct(argv[i]))
      ht_delete(ht, argv[i]);
    else
      is_err = true;
  }

  return BUILTIN_ERROR * is_err;
}