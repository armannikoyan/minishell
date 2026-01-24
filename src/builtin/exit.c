#include <ctype.h>
#include <errno.h>
#include <stdlib.h>

#include "hash_table.h"
#include "builtin.h"
#include "error_codes.h"
#include "utils.h"


static int get_error_status(const char *s) {
    int len;
    int start;
    int value;

    len = 0;
    while (s[len]) {
        if (!isdigit(s[len])) {
            print_error("exit: ", true);
            print_error(s, true);
            print_error(": numeric argument required\n", true);
            return -1;
        }
        len++;
    }

    start = len - 3;
    if (start < 0)
        start = 0;

    value = 0;
    while (start < len)
        value = value * 10 + (s[start++] - '0');

    if (value > 255)
        return -1;

    return value;
}

// Terminates the program with a status.
// If status passed as an argument checks is all the symbols are digits and is last three of them in 0-255 interval.
// If passed more than 1 argument or status not in interval terminates a program with status 2.
// If no arguments passed terminates a program with status of the last command executed (takes status from errno)
int ft_exit(int argc, char **argv, t_hash_table *ht) {
    int error_number;

    (void) ht;
    if (argc == 2) {
        error_number = get_error_status(argv[1]);
        if (error_number < 0)
            exit(2);
        exit(error_number);
    }
    if (argc == 1) {
        exit(errno);
    }
    exit(BUILTIN_ERROR);
}
