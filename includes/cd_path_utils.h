#ifndef CD_PATH_UTILS_H
#define CD_PATH_UTILS_H
#include "hash_table.h"

int check_path(const char *path);

int try_change_dir(const char *path, t_hash_table *ht);

char *normalize_and_resolve_path(const char *path);

void set_oldpwd_and_pwd(const char *pwd, t_hash_table *ht);

char **split_cd_path(const char *str);

#endif
