#include <stdio.h>

#include "hash_table.h"
#include "utils.h"

// Prints all environment variables and finishes with status 0.
// In case of no environment variables prints noting.
// If any arguments passed prints an error and finishes with status 2.
int ft_env(int argc, char **argv, t_hash_table *ht) {
    t_entry *entry;
    int i;

    (void) argv;
    if (argc > 1) {
        //TODO: make normal error
        print_error("env: too many arguments\n", true);
        return 2;
    }
    i = 0;
    while (i < ht->size) {
        entry = ht->buckets[i];
        while (entry) {
            printf("%s=%s\n", entry->key, entry->value);
            entry = entry->next;
        }
        i++;
    }
    return (0);
}
