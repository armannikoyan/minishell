#include <stdlib.h>
#include <string.h>

#include "collections.h"

bool sb_init(t_str_builder *sb, size_t initial_cap) {
  if (initial_cap == 0)
    initial_cap = 16;

  sb->str = malloc(initial_cap);
  if (!sb->str)
    return false;

  sb->str[0] = '\0';
  sb->len = 0;
  sb->capacity = initial_cap;
  return true;
}

static bool sb_ensure_capacity(t_str_builder *sb, const size_t needed_len) {
  if (needed_len < sb->capacity)
    return true;

  size_t new_cap = sb->capacity;
  while (new_cap <= needed_len) {
    new_cap *= 2;
  }

  char *new_str = realloc(sb->str, new_cap);
  if (!new_str)
    return false;

  sb->str = new_str;
  sb->capacity = new_cap;
  return true;
}

bool sb_append(t_str_builder *sb, const char *data, size_t data_len) {
  if (!data || data_len == 0)
    return true;

  if (!sb_ensure_capacity(sb, sb->len + data_len + 1))
    return false;

  memcpy(sb->str + sb->len, data, data_len);
  sb->len += data_len;
  sb->str[sb->len] = '\0';
  return true;
}

bool sb_append_char(t_str_builder *sb, char c) {
  if (!sb_ensure_capacity(sb, sb->len + 2))
    return false;

  sb->str[sb->len++] = c;
  sb->str[sb->len] = '\0';
  return true;
}

void sb_destroy(t_str_builder *sb) {
  free(sb->str);
  sb->str = NULL;
  sb->len = 0;
  sb->capacity = 0;
}

char *sb_extract(t_str_builder *sb) {
  char *result = sb->str;
  sb->str = NULL;
  sb->len = 0;
  sb->capacity = 0;
  return result;
}

bool str_arr_init(t_str_array *arr, size_t initial_cap) {
  if (initial_cap == 0)
    initial_cap = 16;

  arr->data = malloc(sizeof(char *) * initial_cap);
  if (!arr->data)
    return false;

  arr->count = 0;
  arr->capacity = initial_cap;
  return true;
}

bool str_arr_add(t_str_array *arr, char *str) {
  if (arr->count + 1 >= arr->capacity) {
    const size_t new_cap = arr->capacity * 2;
    char **new_data = realloc(arr->data, sizeof(char *) * new_cap);
    if (!new_data)
      return false;

    arr->data = new_data;
    arr->capacity = new_cap;
  }

  arr->data[arr->count++] = str;
  return true;
}

void str_arr_destroy(t_str_array *arr) {
  if (!arr->data)
    return;

  for (size_t i = 0; i < arr->count; i++) {
    free(arr->data[i]);
  }
  free(arr->data);
  arr->data = NULL;
  arr->count = 0;
  arr->capacity = 0;
}

char **str_arr_extract(t_str_array *arr) {
  if (!arr->data)
    return NULL;

  arr->data[arr->count] = NULL;
  char **result = arr->data;

  arr->data = NULL;
  arr->count = 0;
  arr->capacity = 0;
  return result;
}