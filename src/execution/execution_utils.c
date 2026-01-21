#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/errno.h>
#include <sys/wait.h>

#include "error_codes.h"
#include "tokenization.h"
#include "utils.h"

static size_t   get_clean_len(char *str)
{
    size_t  len;
    size_t  i;
    char    quote_state;
    char    prev_state;

    len = 0;
    i = 0;
    quote_state = 0;
    while (str[i])
    {
        prev_state = quote_state;
        set_quote_char(str[i], &quote_state);
        if (prev_state == quote_state)
            len++;
        i++;
    }
    return (len);
}

char    *remove_quotes(char *str)
{
    char    *new_str;
    char    quote_state;
    char    prev_state;
    size_t  i;
    size_t  j;

    if (!str)
        return (NULL);
    new_str = (char *)malloc(sizeof(char) * (get_clean_len(str) + 1));
    if (!new_str)
        return (NULL);
    i = 0;
    j = 0;
    quote_state = 0;
    while (str[i])
    {
        prev_state = quote_state;
        set_quote_char(str[i], &quote_state);
        if (prev_state == quote_state)
            new_str[j++] = str[i];
        i++;
    }
    new_str[j] = '\0';
    return (new_str);
}

void handle_child_exit(pid_t pid) {
    int status;

    if (waitpid(pid, &status, 0) == -1) {
        if (errno != ECHILD)
            print_error("minishell: waitpid", false);
        return;
    }
    if (WIFEXITED(status)) {
        errno = WEXITSTATUS(status);
    }
    else if (WIFSIGNALED(status)) {
        int sig = WTERMSIG(status);
        errno = RESERVED_ERROR_CODES + sig;

        if (sig == SIGINT || sig == SIGPIPE)
            printf("^C\n");
        else if (sig == SIGQUIT)
            printf("^\\Quit: signum: %d\n", sig);
    }
}