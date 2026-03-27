#ifndef COLLECTIONS_H
#define COLLECTIONS_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
  char *str;
  size_t len;
  size_t capacity;
} t_str_builder;

bool sb_init(t_str_builder *sb, size_t initial_cap);
bool sb_append(t_str_builder *sb, const char *data, size_t data_len);
bool sb_append_char(t_str_builder *sb, char c);
void sb_destroy(t_str_builder *sb);
char *sb_extract(t_str_builder *sb);

typedef struct {
  char **data;
  size_t count;
  size_t capacity;
} t_str_array;

bool str_arr_init(t_str_array *arr, size_t initial_cap);
bool str_arr_add(t_str_array *arr, char *str);
void str_arr_destroy(t_str_array *arr);
char **str_arr_extract(t_str_array *arr);

#endif