#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>

#include "builtin.h"
#include "error_codes.h"
#include "hash_table.h"
#include "utils.h"

static int get_error_status(const char *s) {
  int start = 0;
  int len = 0;
  int sign = 1;
  if (s[len] == '+' || s[len] == '-') {
    if (s[len] == '-')
      sign = -1;
    ++start;
    ++len;
  }

  while (s[len]) {
    if (!isdigit(s[len])) {
      print_error("exit: ", true);
      print_error(s, true);
      print_error(": numeric argument required\n", true);
      return -1;
    }

    ++len;
  }

  int value = 0;
  while (start < len)
    value = value * 10 + (s[start++] - '0');

  value = (value * sign) & 0xFF;

  return value;
}

// Terminates the program with a status.
// If status passed as an argument checks is all the symbols are digits and is
// last three of them in 0-255 interval. If passed more than 1 argument or
// status not in interval terminates a program with status 2. If no arguments
// passed terminates a program with status of the last command executed (takes
// status from errno)
int ft_exit(const int argc, char **argv, t_hash_table *ht __attribute((unused)),
            const int errnum) {
  if (argc == 2) {
    if (argv[1][0] == '\0') {
      print_error("exit: numeric argument required\n", true);
      return BUILTIN_ERROR;
    }

    const int error_number = get_error_status(argv[1]);
    if (error_number < 0)
      return BUILTIN_ERROR;

    rl_clear_history();
    exit(error_number);
  }

  if (argc == 1) {
    rl_clear_history();
    exit(errnum);
  }

  print_error("exit: too many arguments\n", true);
  return BUILTIN_ERROR;
}
