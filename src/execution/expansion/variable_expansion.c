#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "collections.h"
#include "expansion.h"
#include "hash_table.h"
#include "tokenization.h"

static bool handle_var_expansion(t_str_builder *sb, const char *str,
                                 size_t *pos, t_hash_table *ht,
                                 const int errnum) {
  ++(*pos);

  if (str[*pos] == '?') {
    char buf[16];
    const int len = snprintf(buf, sizeof(buf), "%d", errnum);
    ++(*pos);
    return sb_append(sb, buf, len);
  }

  const size_t var_name_len = env_var_len(str + *pos);
  if (var_name_len == 0)
    return sb_append_char(sb, '$');

  char *var_name = strndup(str + *pos, var_name_len);
  if (!var_name)
    return false;

  const t_entry *entry = ht_get(ht, var_name);
  free(var_name);

  if (entry && entry->val) {
    if (!sb_append(sb, entry->val, strlen(entry->val)))
      return false;
  }

  *pos += var_name_len;
  return true;
}

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

  t_str_builder sb;
  if (!sb_init(&sb, 64))
    return NULL;

  size_t pos = 0;
  char active_quote = 0;

  while (str[pos]) {
    set_quote_char(str[pos], &active_quote);

    if (str[pos] == '$' && active_quote != '\'') {
      if (str[pos + 1] == '?' || isalpha((unsigned char)str[pos + 1]) ||
          str[pos + 1] == '_') {
        if (!handle_var_expansion(&sb, str, &pos, ht, errnum))
          goto error;
      } else if (active_quote == 0 &&
                 (str[pos + 1] == '\'' || str[pos + 1] == '\"')) {
        ++pos;
      } else {
        if (!sb_append_char(&sb, str[pos++]))
          goto error;
      }
    } else if (is_tilde_expandable(str, pos, active_quote)) {
      const t_entry *home = ht_get(ht, "HOME");

      if (home && home->val) {
        if (!sb_append(&sb, home->val, strlen(home->val)))
          goto error;
      } else {
        if (!sb_append_char(&sb, '~'))
          goto error;
      }
      ++pos;
    } else {
      if (!sb_append_char(&sb, str[pos++]))
        goto error;
    }
  }

  return sb_extract(&sb);

error:
  sb_destroy(&sb);
  return NULL;
}