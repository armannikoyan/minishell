#include <limits.h>
#include <stdio.h>
#include <unistd.h>

#include "builtin.h"
#include "error_codes.h"
#include "hash_table.h"
#include "utils.h"

// Prints current working directory and finishes with status 0.
// If any arguments passed prints an error and finishes with status 2.
int ft_pwd(const int argc __attribute((unused)), char **argv,
           t_hash_table *ht __attribute((unused))) {
  char cwd[PATH_MAX];

  if (argc > 1 && argv[1][0] == '-')
    return BUILTIN_ERROR;

  if (getcwd(cwd, sizeof(cwd)) == NULL) {
    print_error("pwd: getcwd", false);
    return BUILTIN_ERROR;
  }

  printf("%s\n", cwd);

  return 0;
}
