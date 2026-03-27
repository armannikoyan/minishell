#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// If is_custom_err is true, write 'err' directly to stderr by write.
// If the write operation fails, print an error reasons using perror.
// Otherwise, print the 'err' description with perror.
void print_error(const char *err, bool is_custom_err) {
    if (is_custom_err == true) {
        if (write(STDERR_FILENO, err, strlen(err)) == -1)
            perror("minishell: write");
        return;
    }
    perror(err);
}
