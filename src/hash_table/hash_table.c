#include <stdio.h>
#include <stdlib.h>

#include "hash_table.h"
#include "utils.h"

#include <string.h>

t_hash_table *ht_create(void) {
  t_hash_table *ht = malloc(sizeof(t_hash_table));
  if (!ht) {
    print_error("minishell: ht_create: malloc", false);
    return NULL;
  }

  ht->size = INITIAL_SIZE;
  ht->count = 0;
  ht->buckets = calloc(ht->size, sizeof(t_entry *));
  if (!ht->buckets) {
    free(ht);
    print_error("minishell: ht_create: ft_calloc", false);
    return NULL;
  }

  return (ht);
}

void ht_destroy(t_hash_table *ht) {
  if (!ht)
    return;

  for (unsigned long i = 0; i < ht->size; i++) {
    t_entry *entry = ht->buckets[i];
    while (entry) {
      t_entry *next = entry->next;
      free(entry->key);
      free(entry->val);
      free(entry);
      entry = next;
    }
  }
  free(ht->buckets);
  free(ht);
}

t_entry *ht_get(t_hash_table *ht, const char *key) {
  if (!ht || !key)
    return NULL;

  const unsigned long index = hash_func(key) % ht->size;
  t_entry *entry = ht->buckets[index];

  while (entry) {
    if (strcmp(entry->key, key) == 0)
      return entry;
    entry = entry->next;
  }

  return NULL;
}

void ht_resize(t_hash_table *ht, const unsigned long new_size) {
  if (!ht || new_size <= 0)
    return;

  t_entry **new_buckets = calloc(new_size, sizeof(t_entry *));
  if (!new_buckets) {
    print_error("minishell: ht_resize: calloc", false);
    exit(EXIT_FAILURE);
  }

  for (unsigned long i = 0; i < ht->size; i++) {
    t_entry *entry = ht->buckets[i];
    while (entry) {
      t_entry *next = entry->next;
      const unsigned long index = hash_func(entry->key) % new_size;

      entry->next = new_buckets[index];
      new_buckets[index] = entry;
      entry = next;
    }
  }
  free(ht->buckets);
  ht->buckets = new_buckets;
  ht->size = new_size;
}

int ht_create_bucket(t_hash_table *ht, const char *key, const char *value,
                     bool is_local) {
  if (!ht || !key)
    return -1;

  if (ht->size / ht->count <= (int)(MAX_LOAD_FACTOR * 10))
    ht_resize(ht, next_prime(ht->size));

  const unsigned long index = hash_func(key) % ht->size;

  t_entry *new_entry = malloc(sizeof(t_entry));
  if (!new_entry) {
    print_error("minishell: ht_create_bucket: malloc", false);
    return -1;
  }

  new_entry->key = strdup(key);
  if (!new_entry->key) {
    free(new_entry);
    print_error("minishell: ht_create_bucket: strdup key", false);
    return -1;
  }

  if (value != NULL) {
    new_entry->val = strdup(value);
    if (!new_entry->val) {
      free(new_entry->key);
      free(new_entry);
      print_error("minishell: ht_create_bucket: strdup value", false);
      return -1;
    }
  } else
    new_entry->val = NULL;

  new_entry->is_local = is_local;
  new_entry->next = ht->buckets[index];
  ht->buckets[index] = new_entry;
  ht->count++;

  return 0;
}

// 0 - success
// 1 - entry doesn't exist
// 2 - strdup failed
int ht_update_value(t_hash_table *ht, const char *key, const char *value) {
  if (!ht || !key)
    return 1;

  t_entry *entry = ht_get(ht, key);
  if (!entry)
    return 1;

  char *new_value = NULL;
  if (value != NULL) {
    new_value = strdup(value);
    if (!new_value) {
      print_error("minishell: ht_update_value: strdup", false);
      return 2;
    }
  }

  free(entry->val);
  entry->val = new_value;

  return 0;
}

void ht_delete(t_hash_table *ht, const char *key) {
  if (!ht || !key)
    return;

  const unsigned long index = hash_func(key) % ht->size;
  t_entry *entry = ht->buckets[index];
  t_entry *prev = NULL;

  while (entry) {
    if (strcmp(entry->key, key) == 0) {
      if (prev)
        prev->next = entry->next;
      else
        ht->buckets[index] = entry->next;

      free(entry->key);
      free(entry->val);
      free(entry);
      ht->count--;

      if (ht->size > MIN_SIZE &&
          ht->size / ht->count >= (int)(MIN_LOAD_FACTOR * 10))
        ht_resize(ht, prev_prime(ht->size));
      return;
    }

    prev = entry;
    entry = entry->next;
  }
}
