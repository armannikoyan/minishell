#ifndef BUILTIN_H
#define BUILTIN_H

#include "hash_table.h"

typedef int (*t_builtin_func)(int argc, char **argv, t_hash_table *ht,
                              int errnum);

typedef struct {
  const char *name;
  t_builtin_func func;
} t_builtin_def;

int ft_cd(int argc, char **argv, t_hash_table *ht, int errnum);

int ft_echo(int argc, char **argv, t_hash_table *ht, int errnum);

int ft_env(int argc, char **argv, t_hash_table *ht, int errnum);

int ft_exit(int argc, char **argv, t_hash_table *ht, int errnum);

int ft_export(int argc, char **argv, t_hash_table *ht, int errnum);

int ft_pwd(int argc, char **argv, t_hash_table *ht, int errnum);

int ft_unset(int argc, char **argv, t_hash_table *ht, int errnum);

#endif
