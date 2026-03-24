#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "ast.h"
#include "builtin.h"
#include "collections.h"
#include "error_codes.h"
#include "execution.h"
#include "expansion.h"
#include "hash_table.h"
#include "path_utils.h"
#include "term_settings.h"
#include "utils.h"

static void update_underscore(t_hash_table *ht, const char *value) {
  if (ht_get(ht, "_") == NULL)
    ht_create_bucket(ht, "_", value, false);
  else
    ht_update_value(ht, "_", value);
}

static char **ht_to_envp(const t_hash_table *ht) {
  t_str_array env_arr;

  if (!str_arr_init(&env_arr, ht->count + 1))
    return NULL;

  for (size_t i = 0; i < (size_t)ht->size; i++) {
    for (const t_entry *entry = ht->buckets[i]; entry; entry = entry->next) {
      const size_t klen = strlen(entry->key);
      const size_t vlen = entry->val ? strlen(entry->val) : 0;

      char *env_str = malloc(klen + vlen + 2);
      if (!env_str) {
        str_arr_destroy(&env_arr);
        return NULL;
      }

      memcpy(env_str, entry->key, klen);
      env_str[klen] = '=';
      if (entry->val)
        memcpy(env_str + klen + 1, entry->val, vlen);
      env_str[klen + 1 + vlen] = '\0';

      if (!str_arr_add(&env_arr, env_str)) {
        free(env_str);
        str_arr_destroy(&env_arr);
        return NULL;
      }
    }
  }
  return str_arr_extract(&env_arr);
}

static char *search_in_path_env(const char *cmd, t_hash_table *ht) {
  const t_entry *path_entry = ht_get(ht, "PATH");
  if (!path_entry || !path_entry->val)
    return NULL;

  const char *start = path_entry->val;
  const size_t cmd_len = strlen(cmd);

  while (*start) {
    const char *end = strchr(start, ':');
    if (!end)
      end = start + strlen(start);

    size_t dir_len = end - start;
    char *full_path = malloc(dir_len + cmd_len + 2);

    if (full_path) {
      if (dir_len > 0) {
        memcpy(full_path, start, dir_len);
        full_path[dir_len] = '/';
        memcpy(full_path + dir_len + 1, cmd, cmd_len + 1);
      } else {
        memcpy(full_path, cmd, cmd_len + 1);
      }

      if (access(full_path, F_OK) == 0)
        return full_path;

      free(full_path);
    }

    start = *end ? end + 1 : end;
  }
  return NULL;
}

static char *resolve_cmd_path(char *cmd, t_hash_table *ht) {
  if (!cmd || !*cmd)
    return NULL;

  if (strchr(cmd, '/'))
    return strdup(cmd);

  char *cmd_path = search_in_path_env(cmd, ht);
  return cmd_path ? cmd_path : strdup(cmd);
}

static int run_expansion(t_ast_node *node, t_hash_table *ht, const int errnum) {
  for (size_t i = 0; node->u_data.cmd.argv[i]; i++) {
    char *tmp = node->u_data.cmd.argv[i];
    node->u_data.cmd.argv[i] = expand_dollar_sign(tmp, ht, errnum);
    free(tmp);
    if (!node->u_data.cmd.argv[i]) {
      print_error("minishell: malloc", false);
      return 1;
    }
  }

  char **new_argv = expand_wildcards(node->u_data.cmd.argv);
  if (!new_argv) {
    print_error("minishell: malloc", false);
    return 1;
  }

  free_split(node->u_data.cmd.argv);
  node->u_data.cmd.argv = new_argv;
  return 0;
}

static int count_args(char **argv) {
  int i = 0;
  while (argv[i])
    i++;

  return i;
}

static int run_builtin(char **argv, t_hash_table *ht, const int errnum) {
  if (!argv || !argv[0])
    return -1;

  static const t_builtin_def builtins[] = {
      {"echo", ft_echo},     {"cd", ft_cd},       {"pwd", ft_pwd},
      {"export", ft_export}, {"unset", ft_unset}, {"env", ft_env},
      {"exit", ft_exit},     {NULL, NULL}};

  const char *cmd = argv[0];
  const int argc = count_args(argv);

  for (int i = 0; builtins[i].name != NULL; i++) {
    if (strcmp(cmd, builtins[i].name) == 0) {
      update_underscore(ht, cmd);
      return builtins[i].func(argc, argv, ht, errnum);
    }
  }

  return -1;
}

static int validate_executable(const char *path) {
  const size_t mask = check_access(path);

  if (!(mask & EXISTS_BIT)) {
    print_error("minishell: ", true);
    print_error(path, true);
    print_error(": command not found\n", true);
    return COMMAND_NOT_FOUND;
  }
  if (!(mask & FILE_BIT)) {
    print_error("minishell: ", true);
    print_error(path, true);
    print_error(": Is a directory\n", true);
    return IS_A_DIRECTORY;
  }
  if (!(mask & EXEC_BIT)) {
    print_error("minishell: ", true);
    print_error(path, true);
    print_error(": Permission denied\n", true);
    return PERMISSION_DENIED;
  }
  return 0;
}

static void execute_child(const char *cmd_path, char **argv,
                          const t_hash_table *ht) {
  signal(SIGINT, SIG_DFL);
  signal(SIGQUIT, SIG_DFL);

  char **envp = ht_to_envp(ht);
  if (!envp)
    exit(EXIT_FAILURE);

  execve(cmd_path, argv, envp);

  print_error("minishell: ", true);
  print_error(argv[0], true);
  print_error(": ", false);

  free_split(envp);

  if (errno == ENOENT)
    exit(COMMAND_NOT_FOUND);
  else if (errno == EACCES)
    exit(PERMISSION_DENIED);
  exit(1);
}

int execute_command(t_ast_node *node, t_hash_table *ht, const int errnum) {
  if (run_expansion(node, ht, errnum) != 0)
    return 1;

  if (!node->u_data.cmd.argv || !node->u_data.cmd.argv[0])
    return 0;

  int err_code = run_builtin(node->u_data.cmd.argv, ht, errnum);
  if (err_code != -1)
    return err_code;

  char *cmd_path = resolve_cmd_path(node->u_data.cmd.argv[0], ht);
  update_underscore(ht, cmd_path);

  err_code = validate_executable(cmd_path);
  if (err_code != 0) {
    free(cmd_path);
    return err_code;
  }

  pid_t pid = fork();
  if (pid == -1) {
    print_error("minishell: fork", false);
    free(cmd_path);
    return 1;
  }

  if (pid == 0) {
    execute_child(cmd_path, node->u_data.cmd.argv, ht);
  } else {
    free(cmd_path);
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    err_code = handle_child_exit(pid);
    psig_set();
    return err_code;
  }
  return 1;
}