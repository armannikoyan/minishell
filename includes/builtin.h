#ifndef BUILTIN_H
#define BUILTIN_H

#include "hash_table.h"

int ft_cd(int argc, char **argv, t_hash_table *ht);

int ft_echo(int argc, char **argv, t_hash_table *ht);

int ft_env(int argc, char **argv, t_hash_table *ht);

int ft_exit(int argc, char **argv, t_hash_table *ht);

int ft_export(int argc, char **argv, t_hash_table *ht);

int ft_pwd(int argc, char **argv, t_hash_table *ht);

int ft_unset(int argc, char **argv, t_hash_table *ht);

#endif
