#include <stdio.h>

#include "hash_table.h"
#include "utils.h"

static void inner_circle(t_entry *entry) {
    while (entry) {
        if (entry->is_local == false) {
            if (entry->val == NULL)
                printf("%s\n", entry->key);
            else if (entry->val[0] == '\0')
                printf("%s=""\n", entry->key);
            else
                printf("%s=%s\n", entry->key, entry->val);
        }
        entry = entry->next;
    }
}

// Prints all environment variables and finishes with status 0.
// In case of no environment variables prints noting.
// If any arguments passed prints an error and finishes with status 2.
int ft_env(int argc, char **argv, t_hash_table *ht) {
    int i;

    (void) argv;
    //TODO: make normal error
    if (argc > 1)
        return (print_error("env: too many arguments\n", true), 2);
    i = 0;
    while (i < ht->size) {
        inner_circle(ht->buckets[i]);
        i++;
    }
    return (0);
}
