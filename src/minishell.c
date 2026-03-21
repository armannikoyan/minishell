#include <ctype.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syslimits.h>
#include <unistd.h>

#include "builtin.h"
#include "error_codes.h"
#include "execution.h"
#include "get_next_line.h"
#include "hash_table.h"
#include "minishell.h"
#include "tokenization.h"
#include "utils.h"

static void update_shlvl(t_hash_table *ht) {
  int shlvl_val = 1;
  char buffer[32];

  if (ht_get(ht, "SHLVL") == NULL) {
    if (ht_create_bucket(ht, "SHLVL", NULL, false) == -1)
      exit(EXIT_FAILURE);
  }

  const t_entry *entry = ht_get(ht, "SHLVL");
  const char *shlvl_str = entry ? entry->val : NULL;

  if (shlvl_str) {
    if (strtoi(shlvl_str, &shlvl_val))
      shlvl_val += 1;
    else
      shlvl_val = 1;
  }

  snprintf(buffer, sizeof(buffer), "%d", shlvl_val);
  ht_update_value(ht, "SHLVL", buffer);
}

static void replace_incorrect_env(t_hash_table *ht) {
  char str[PATH_MAX];

  update_shlvl(ht);
  if (getcwd(str, sizeof(str)) == NULL)
    print_error("minishell: replace_incorrect_env: getcwd", false);
  if (ht_get(ht, "PWD") == NULL)
    ht_create_bucket(ht, "PWD", str, false);
  else
    ht_update_value(ht, "PWD", str);
  if (ht_get(ht, "OLDPWD") == NULL)
    ht_create_bucket(ht, "OLDPWD", NULL, false);
}

static void insert_env(t_hash_table *ht, char **envp) {
  size_t i = 0;
  size_t j = 0;

  while (envp[i]) {
    // TODO: Тут можно попробовать сделать выделение одного буфера под все
    // ключи. Пример найдёшь в экспорте
    j = strchr(envp[i], '=') - envp[i];
    char *str = (char *)malloc(sizeof(char) * (j + 1));
    if (!str) {
      print_error("minishell: insert_env: malloc", false);
      exit(EXIT_FAILURE);
    }
    strlcpy(str, envp[i], j + 1);
    ht_create_bucket(ht, str, &envp[i][j + 1], false);
    free(str);
    i++;
  }
  replace_incorrect_env(ht);
}

static char *trim(const char *str) {
  if (!str)
    return NULL;

  while (isspace((unsigned char)*str))
    str++;

  if (*str == '\0')
    return strdup("");

  const char *end = str + strlen(str) - 1;
  while (end > str && isspace((unsigned char)*end))
    end--;

  size_t len = end - str + 1;
  char *trimmed = (char *)malloc(len + 1);
  if (!trimmed)
    return NULL;

  memcpy(trimmed, str, len);
  trimmed[len] = '\0';
  return trimmed;
}

static char *read_input(void) {
  char *raw_input;

  if (isatty(STDIN_FILENO))
    raw_input = readline("minishell$ ");
  else
    raw_input = get_next_line(STDIN_FILENO);

  char *input = trim(raw_input);
  free(raw_input);
  return input;
}

static void handle_eof(t_hash_table *ht, int *eof_count, const int errnum) {
  (*eof_count)--;
  if (*eof_count < 0) {
    char *exit_args[] = {"exit", NULL};
    ft_exit(1, exit_args, ht, errnum);
  } else
    printf("Use \"exit\" to leave the shell.\n");
}

static bool is_all_space(const char *str) {
  size_t i = 0;
  while (str[i]) {
    if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
      return (false);
    ++i;
  }
  return (true);
}

static int process_input(char *input, t_hash_table *ht, int errnum) {
  int heredoc_counter = 0;

  if (is_all_space(input) || input[0] == '\0')
    return errnum;

  add_history(input);
  t_ast_node *root = tokenize(input, &errnum);

  if (root != NULL &&
      scan_and_process_heredocs(root, ht, &heredoc_counter) == 0 &&
      syntax_check(root, &errnum) != SYNTAX_ERROR)
    errnum = execute(root, ht, errnum);

  cleanup_heredoc_files(heredoc_counter);
  ast_deletion(root);

  return errnum;
}

static void update_eof_count(t_hash_table *ht, int *eof_count) {
  const t_entry *ignore_eof = ht_get(ht, "IGNOREEOF");

  if (ignore_eof == NULL || !strtoi(ignore_eof->val, eof_count))
    *eof_count = 0;
}

void interactive_loop(char **envp) {
  int errnum = 0;
  int eof_count = 0;

  t_hash_table *ht = ht_create();
  if (!ht)
    exit(EXIT_FAILURE);

  insert_env(ht, envp);
  update_eof_count(ht, &eof_count);

  // ReSharper disable once CppDFAEndlessLoop
  while (true) {
    char *input = read_input();

    if (!input) {
      handle_eof(ht, &eof_count, errnum);
      continue;
    }

    errnum = process_input(input, ht, errnum);
    update_eof_count(ht, &eof_count);

    free(input);
  }
}
