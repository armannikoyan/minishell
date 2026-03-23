#include <stdio.h>

#include "builtin.h"
#include "error_codes.h"
#include "hash_table.h"
#include "utils.h"

// Prints all environment variables and finishes with status 0.
// In case of no environment variables prints noting.
// If any arguments passed prints an error and finishes with status 2.
int ft_env(const int argc, char **argv __attribute__((unused)),
           const t_hash_table *ht __attribute((unused))) {
  if (argc > 1)
    return (print_error("env: too many arguments\n", true), BUILTIN_ERROR);

  for (unsigned long i = 0; i < ht->size; i++) {
    const t_entry *entry = ht->buckets[i];

    while (entry) {
      if (entry->is_local == false && entry->val != NULL)
        printf("%s=%s\n", entry->key, entry->val);

      entry = entry->next;
    }
  }

  return 0;
}
