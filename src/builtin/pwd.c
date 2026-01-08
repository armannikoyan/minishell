#include <stdio.h>
#include <unistd.h>
#include <linux/limits.h>

#include "hash_table.h"
#include "utils.h"

// Prints current working directory and finishes with status 0.
// If any arguments passed prints an error and finishes with status 2.
int ft_pwd(int argc, char **argv, t_hash_table *ht) {
    char cwd[PATH_MAX];

    (void) argv;
    (void) ht;
    if (argc > 1) {
        //TODO: make normal error
        print_error("pwd: too many arguments\n", true);
        return 2;
    }
    if (getcwd(cwd, PATH_MAX) == NULL) {
        //TODO: make normal error
        print_error("pwd: getcwd", false);
        return 2;
    }
    printf("%s\n", cwd);
    return 0;
}
