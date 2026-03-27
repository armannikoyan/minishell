#include <dirent.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "collections.h"
#include "execution.h"
#include "tokenization.h"

static int compare_strings(const void *a, const void *b) {
  return strcmp(*(const char **)a, *(const char **)b);
}

static char *extract_pattern_and_mask(const char *raw_pattern, int **out_mask) {
  const size_t len = strlen(raw_pattern);
  char *unquoted_pattern = calloc(len + 1, sizeof(char));
  if (!unquoted_pattern)
    return NULL;

  int *quote_mask = calloc(len + 1, sizeof(int));
  if (!quote_mask) {
    free(unquoted_pattern);
    return NULL;
  }

  size_t read_idx = 0;
  size_t write_idx = 0;
  char active_quote = 0;

  while (raw_pattern[read_idx]) {
    const char old_quote = active_quote;
    set_quote_char(raw_pattern[read_idx], &active_quote);

    if (old_quote != active_quote) {
      read_idx++;
      continue;
    }
    unquoted_pattern[write_idx] = raw_pattern[read_idx];
    quote_mask[write_idx] = (active_quote != 0) ? 1 : 0;
    write_idx++;
    read_idx++;
  }
  *out_mask = quote_mask;
  return unquoted_pattern;
}

static bool is_pattern_matching(const char *pattern, const int *quote_mask,
                                const char *filename) {
  if (*pattern == '\0' && *filename == '\0')
    return true;

  if (*pattern == '*' && *quote_mask == 0) {
    if (is_pattern_matching(pattern + 1, quote_mask + 1, filename))
      return true;
    if (*filename && is_pattern_matching(pattern, quote_mask, filename + 1))
      return true;
    return false;
  }

  if (*pattern == *filename)
    return is_pattern_matching(pattern + 1, quote_mask + 1, filename + 1);

  return false;
}

static void collect_matching_files(const char *raw_pattern,
                                   t_str_array *matches) {
  int *quote_mask = NULL;
  char *clean_pattern = extract_pattern_and_mask(raw_pattern, &quote_mask);
  if (!clean_pattern)
    return;

  DIR *dir = opendir(".");
  if (!dir) {
    free(clean_pattern);
    free(quote_mask);
    return;
  }

  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
      continue;

    if (entry->d_name[0] == '.' && clean_pattern[0] != '.')
      continue;

    if (is_pattern_matching(clean_pattern, quote_mask, entry->d_name)) {
      char *dup = strdup(entry->d_name);
      if (dup && !str_arr_add(matches, dup)) {
        free(dup);
      }
    }
  }

  closedir(dir);
  free(clean_pattern);
  free(quote_mask);

  if (matches->count > 0)
    qsort(matches->data, matches->count, sizeof(char *), compare_strings);
}

char **expand_wildcards(char **args) {
  t_str_array expanded_args;
  if (!str_arr_init(&expanded_args, 16))
    return NULL;

  for (size_t i = 0; args[i] != NULL; ++i) {
    bool success = true;

    if (strchr(args[i], '*')) {
      t_str_array matches;
      if (!str_arr_init(&matches, 16)) {
        str_arr_destroy(&expanded_args);
        return NULL;
      }

      collect_matching_files(args[i], &matches);

      if (matches.count > 0) {
        for (size_t j = 0; j < matches.count; j++) {
          if (!success || !str_arr_add(&expanded_args, matches.data[j])) {
            success = false;
            free(matches.data[j]);
          }
        }
        free(matches.data);
      } else {
        str_arr_destroy(&matches);
        char *unquoted = remove_quotes(args[i]);
        if (!unquoted || !str_arr_add(&expanded_args, unquoted)) {
          free(unquoted);
          success = false;
        }
      }
    } else {
      char *unquoted = remove_quotes(args[i]);
      if (!unquoted || !str_arr_add(&expanded_args, unquoted)) {
        free(unquoted);
        success = false;
      }
    }

    if (!success) {
      str_arr_destroy(&expanded_args);
      return NULL;
    }
  }

  return str_arr_extract(&expanded_args);
}