#ifndef PATH_UTILS_H
#define PATH_UTILS_H
#include "hash_table.h"

// To check is the bit equals 1 use this:
// #define IS_EXISTS(x)       ((x) & EXISTS_BIT)
// Thanks to idiotic rules of norminette you can't just use this macros

// Sets existence
#define EXISTS_BIT      0b00000001
// Sets stat errors
#define STAT_FAIL_BIT   0b00000010
// Sets file types
#define DIR_BIT         0b00000100
#define FILE_BIT        0b00001000
// Sets permissions
#define READ_BIT        0b00010000
#define WRITE_BIT       0b00100000
#define EXEC_BIT        0b01000000


char *concat_path(const char *p1, const char *p2);

char *remove_last_dir(char *path);

char *expand_tilda(const char *path, t_hash_table *ht);

int check_access(const char *path);

#endif
