#ifndef UTILS_H
# define UTILS_H

# include <stdbool.h>

void free_split(char **cd_path);

// --------- ERROR PRINTING ---------
void print_error(const char *err, bool is_custom_err);

#endif
