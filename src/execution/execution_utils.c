#include <unistd.h>
#include <sys/errno.h>
#include <sys/wait.h>

#include "utils.h"

void    handle_child_exit(pid_t pid) {
    int status;

    if (waitpid(pid, &status, 0) == -1) {
        // TODO: write normal error
        print_error("minishell: waitpid", false);
    }
    if (WIFEXITED(status))
        errno = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
    {
        errno = 128 + WTERMSIG(status);
        if (WTERMSIG(status) == SIGINT)
            print_error("\n", true);
        else if (WTERMSIG(status) == SIGQUIT)
            print_error("Quit (core dumped)\n", true);
    }
}