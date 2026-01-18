#ifndef UTILS_H
# define UTILS_H

# include <stdbool.h>

// To check is the bit equal to 1 use this:
// #define IS_EXISTS(x)       ((x) & EXISTS_BIT)
// Thanks to idiotic rules of norminette you can't just use this macros

// Sets existence
#define EXISTS_BIT     0b00000001
// Sets file types
#define DIR_BIT        0b00000010
#define FILE_BIT       0b00000100
// Sets permissions
#define READ_BIT       0b00001000
#define WRITE_BIT      0b00010000
#define EXEC_BIT       0b00100000
// Sets stat errors
#define STAT_FAIL_BIT  0b01000000


void free_split(char **cd_path);

char **split_env_var(const char *str);

int check_access(const char *path);

// --------- ERROR PRINTING ---------
void print_error(const char *err, bool is_custom_err);

#endif