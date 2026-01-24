#include <stdio.h>
#include <unistd.h>
#include <limits.h>

#include "hash_table.h"
#include "utils.h"
#include "builtin.h"
#include "error_codes.h"

// Prints current working directory and finishes with status 0.
// If any arguments passed prints an error and finishes with status 2.
int ft_pwd(int argc, char **argv, t_hash_table *ht) {
    char cwd[PATH_MAX];

    (void) argv;
    (void) ht;
    if (argc > 1 && argv[1][0] == '-')
        return BUILTIN_ERROR;
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        //TODO: make normal error
        print_error("pwd: getcwd", false);
        return BUILTIN_ERROR;
    }
    printf("%s\n", cwd);
    return 0;
}
