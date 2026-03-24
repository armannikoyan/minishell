#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error_codes.h"
#include "hash_table.h"
#include "utils.h"

static bool is_ev_correct(const char *str, size_t *len) {
  if (!str || !(isalpha(str[0]) || str[0] == '_'))
    return false;
  ++(*len);
  while (str[*len] && (isalnum(str[*len]) || str[*len] == '_'))
    ++(*len);
  return true;
}

static void fantastic_25_lines(char *key, const size_t len, t_hash_table *ht,
                               char *argv) {
  if (argv[len] == '\0' && ht_get(ht, argv) == NULL)
    ht_create_bucket(ht, argv, NULL, false);
  else if (argv[len] == '\0')
    ht_get(ht, argv)->is_local = false;
  if (argv[len] == '=' && argv[len + 1] == '\0') {
    argv[len] = '\0';
    if (ht_get(ht, argv) == NULL)
      ht_create_bucket(ht, argv, "", false);
    else {
      ht_get(ht, argv)->is_local = false;
      ht_update_value(ht, argv, "");
    }
  } else if (argv[len] == '=') {
    memcpy(key, argv, len);
    key[len] = '\0';
    if (ht_get(ht, key) == NULL)
      ht_create_bucket(ht, key, argv + len + 1, false);
    else {
      ht_get(ht, key)->is_local = false;
      ht_update_value(ht, key, argv + len + 1);
    }
  }
}

static bool insert(char *key, t_hash_table *ht, char **argv) {
  bool is_err = false;
  for (int i = 1; argv[i]; ++i) {
    size_t len = 0;

    if (!is_ev_correct(argv[i], &len)) {
      is_err = true;
      (print_error("export: `", true), print_error(argv[i], true));
      print_error("\': not a valid identifier\n", true);
      continue;
    }
    fantastic_25_lines(key, len, ht, argv[i]);
    if (argv[i][len] != '\0' && argv[i][len] != '=') {
      is_err = true;
      (print_error("export: `", true), print_error(argv[i], true));
      print_error("\': not a valid identifier\n", true);
    }
  }

  return is_err;
}

static void print_exported(const t_hash_table *ht) {
  for (unsigned long i = 0; i < ht->size; ++i) {
    const t_entry *entry = ht->buckets[i];

    while (entry) {
      if (entry->is_local == false && strcmp("_", entry->key) != 0) {
        if (entry->val == NULL)
          printf("declare -x %s\n", entry->key);
        else if (entry->val[0] == '\0')
          printf("declare -x %s=\"\"\n", entry->key);
        else
          printf("declare -x %s=\"%s\"\n", entry->key, entry->val);
      }
      entry = entry->next;
    }
  }
}

int ft_export(const int argc, char **argv, t_hash_table *ht,
              const int errnum __attribute__((unused))) {

  if (argc == 1)
    return (print_exported(ht), 0);

  size_t longest = 0;
  for (size_t i = 1; argv[i]; ++i) {
    size_t tmp = 0;
    if (is_ev_correct(argv[i], &tmp) && tmp > longest)
      longest = tmp;
  }

  char *key = calloc(longest + 1, sizeof(char));
  if (key == NULL) {
    print_error("export: malloc", false);
    return 2;
  }

  if (insert(key, ht, argv)) {
    free(key);
    return BUILTIN_ERROR;
  }

  free(key);
  return 0;
}
