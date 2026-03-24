#include <stdbool.h>
#include <stdio.h>

#include "builtin.h"
#include "hash_table.h"

static bool is_n_flag(const char *s) {
  if (s == NULL || s[0] == '\0')
    return false;

  if (s[0] != '-' || s[1] != 'n')
    return false;

  size_t i = 2;
  while (s[i] && s[i] == 'n')
    i++;

  if (s[i] != '\0')
    return false;

  return true;
}

// Prints all passed arguments and finishes with status 0.
// In case of option '-n' passed doesn't add '\n' at the end.
int ft_echo(const int argc, char **argv, t_hash_table *ht __attribute((unused)),
            const int errnum __attribute((unused))) {
  int i = 1;
  bool n_flag = false;

  if (is_n_flag(argv[i])) {
    n_flag = true;
    i++;
  }

  bool is_first_print = false;
  while (i < argc) {
    if (is_n_flag(argv[i++]) && !is_first_print)
      continue;

    is_first_print = true;
    printf("%s", argv[--i]);

    if (i++ != argc - 1)
      printf(" ");
  }

  if (!n_flag)
    printf("\n");

  return 0;
}
