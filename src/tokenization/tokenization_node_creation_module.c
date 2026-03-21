#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "error_codes.h"
#include "tokenization.h"
#include "utils.h"

static void skip_spaces(const char *input, size_t *pos) {
  while (isspace((unsigned char)input[*pos]))
    ++(*pos);
}

static bool is_token_end(const char *input, const size_t pos,
                         const char active_quote) {
  return active_quote == 0 &&
         (isspace((unsigned char)input[pos]) || is_operator(input + pos) ||
          is_redir(input + pos));
}

static void skip_word(const char *input, size_t *pos, char *active_quote) {
  while (input[*pos]) {
    set_quote_char(input[*pos], active_quote);
    if (is_token_end(input, *pos, *active_quote))
      break;
    ++(*pos);
  }
}

/*
** Helper to skip over a redirection operator and its filename.
** Updates the index 'j' to point after the filename.
*/
static void skip_redirection(const char *input, size_t *pos) {
  char active_quote = 0;

  if ((input[*pos] == '<' || input[*pos] == '>') &&
      input[*pos] == input[*pos + 1])
    *pos += 2;
  else
    ++(*pos);

  skip_spaces(input, pos);
  skip_word(input, pos, &active_quote);
}

static void get_subshell_nesting_info(const char *input, size_t *pos,
                                      int *parens_depth) {
  char active_quote = 0;

  while (input[*pos]) {
    set_quote_char(input[*pos], &active_quote);
    if (active_quote == 0) {
      if (input[*pos] == '(')
        ++(*parens_depth);
      else if (input[*pos] == ')') {
        --(*parens_depth);
        if (*parens_depth == 0)
          break;
      }
    }
    ++(*pos);
  }
}

char *extract_subshell_content(const char *input, size_t *pos, int *errnum) {
  const size_t content_start = ++(*pos);
  int parens_depth = 1;

  get_subshell_nesting_info(input, pos, &parens_depth);
  if (parens_depth != 0) {
    print_error("minishell: parsing error near unexpected token `(\'\n", true);
    *errnum = SYNTAX_ERROR;
    return NULL;
  }

  char *content = strndup(input + content_start, *pos - content_start);

  if (input[*pos] == ')')
    (*pos)++;

  return content;
}

char *substr_next(const char *input, size_t *pos) {
  char active_quote = 0;

  skip_spaces(input, pos);
  const size_t token_start = *pos;
  skip_word(input, pos, &active_quote);

  return strndup(input + token_start, *pos - token_start);
}

static size_t get_argv_len(const char *input, const size_t *start_pos) {
  size_t scan_pos = *start_pos;
  size_t arg_count = 0;
  char active_quote;

  while (input[scan_pos] && !is_operator(input + scan_pos)) {
    skip_spaces(input, &scan_pos);

    if (!input[scan_pos] || is_operator(input + scan_pos))
      break;

    if (is_redir(input + scan_pos)) {
      skip_redirection(input, &scan_pos);
      continue;
    }

    arg_count++;
    active_quote = 0;
    skip_word(input, &scan_pos, &active_quote);

    if (active_quote != 0) {
      print_error("minishell: parsing error near unexpected token `", true);
      dprintf(STDERR_FILENO, "%c'\n", active_quote);
      return 0;
    }
  }

  return arg_count;
}

char **get_argv(char *input, size_t *pos) {
  const size_t arg_count = get_argv_len(input, pos);
  if (arg_count == 0)
    return NULL;

  char **argv = (char **)malloc(sizeof(char *) * (arg_count + 1));
  if (!argv) {
    print_error("minishell: malloc", false);
    return NULL;
  }

  size_t scan_pos = *pos;
  bool skipped_redir = false;
  char active_quote;

  for (size_t arg_index = 0; arg_index < arg_count; ++arg_index) {
    skip_spaces(input, &scan_pos);

    if (is_redir(input + scan_pos)) {
      skip_redirection(input, &scan_pos);
      skipped_redir = true;
      continue;
    }

    const size_t arg_start = scan_pos;
    active_quote = 0;
    skip_word(input, &scan_pos, &active_quote);

    argv[arg_index] = strndup(input + arg_start, scan_pos - arg_start);
    if (!argv[arg_index]) {
      free_split(argv);
      return NULL;
    }

    if (!skipped_redir)
      *pos = scan_pos;
    else
      memset(input + arg_start, ' ', scan_pos - arg_start);
  }
  argv[arg_count] = NULL;

  return argv;
}