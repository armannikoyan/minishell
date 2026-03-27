#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/errno.h>

#include "utils.h"

bool strtoi(const char *str, int *out) {
    char *endptr;

    if (!str)
        return false;

    errno = 0;
    const long val = strtol(str, &endptr, 10);

    if (errno == ERANGE || val > INT_MAX || val < INT_MIN)
        return false;

    if (endptr == str || *endptr != '\0')
        return false;

    *out = (int) val;
    return true;
}
