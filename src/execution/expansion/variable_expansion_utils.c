#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

size_t env_var_len(const char *str) {
  if (!str)
    return 0;

  size_t i = 0;
  if (isalpha(str[i]) || str[i] == '_') {
    ++i;

    while (str[i] && (isalnum(str[i]) || str[i] == '_'))
      ++i;
  }

  return i;
}

char *get_env_val(const char *str, t_hash_table *ht) {
  const size_t len = env_var_len(str);

  char *key = strndup(str, len);
  if (!key)
    return NULL;

  const t_entry *entry = ht_get(ht, key);
  if (entry == NULL) {
    free(key);
    return NULL;
  }

  free(key);
  return entry->val;
}