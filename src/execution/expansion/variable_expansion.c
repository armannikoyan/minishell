#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "expansion.h"
#include "hash_table.h"
#include "tokenization.h"

static char *append_to_str(char *old, const char *src, size_t src_len) {
  const size_t old_len = old ? strlen(old) : 0;

  char *new_str = realloc(old, old_len + src_len + 1);
  if (!new_str) {
    free(old);
    return NULL;
  }

  memcpy(new_str + old_len, src, src_len);
  new_str[old_len + src_len] = '\0';
  return new_str;
}

static char *handle_var_expansion(char *res, const char *str, size_t *pos,
                                  t_hash_table *ht, const int errnum) {
  ++(*pos);

  if (str[*pos] == '?') {
    char buf[12];
    const int len = snprintf(buf, sizeof(buf), "%d", errnum);
    ++(*pos);
    return append_to_str(res, buf, len);
  }

  const size_t var_name_len = env_var_len(str + *pos);
  if (var_name_len == 0)
    return append_to_str(res, "$", 1);

  char *var_name = strndup(str + *pos, var_name_len);
  const t_entry *entry = ht_get(ht, var_name);
  free(var_name);

  if (entry && entry->val)
    res = append_to_str(res, entry->val, strlen(entry->val));

  *pos += var_name_len;
  return res;
}

// Tilde expands if:
// 1. Not in quotes
// 2. It is at the start OR preceded by a space
// 3. It is at the end OR followed by a '/'
static int is_tilde_expandable(const char *str, const size_t i,
                               const char quote_char) {
  if (str[i] != '~' || quote_char != 0)
    return 0;
  if ((i == 0 || str[i - 1] == ' ') &&
      (str[i + 1] == '\0' || str[i + 1] == '/'))
    return 1;

  return 0;
}

char *expand_dollar_sign(char *str, t_hash_table *ht, const int errnum) {
  if (!str)
    return NULL;

  char *res = strdup("");
  size_t pos = 0;
  char active_quote = 0;

  while (str[pos]) {
    set_quote_char(str[pos], &active_quote);

    if (str[pos] == '$' && active_quote != '\'') {
      if (str[pos + 1] == '?' || isalpha((unsigned char)str[pos + 1]) ||
          str[pos + 1] == '_') {
        res = handle_var_expansion(res, str, &pos, ht, errnum);
      } else if (active_quote == 0 &&
                 (str[pos + 1] == '\'' || str[pos + 1] == '\"')) {
        ++pos;
      } else
        res = append_to_str(res, str + (pos++), 1);
    } else if (is_tilde_expandable(str, pos, active_quote)) {
      const t_entry *home = ht_get(ht, "HOME");
      if (home && home->val)
        res = append_to_str(res, home->val, strlen(home->val));
      else
        res = append_to_str(res, "~", 1);
      ++pos;
    } else
      res = append_to_str(res, str + (pos++), 1);

    if (!res)
      return NULL;
  }

  return res;
}