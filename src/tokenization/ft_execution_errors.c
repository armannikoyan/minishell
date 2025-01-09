#include "../../includes/minishell.h"

extern int	g_errno;

void	handle_open_error(void)
{
	perror("open failed");
	g_errno = 1;
}

void	parent_wait_and_set_status(pid_t pid, int *status)
{
	waitpid(pid, status, 0);
	if (WIFEXITED(*status))
		g_errno = WEXITSTATUS(*status);
	else
		g_errno = 1;
}

void	handle_fork_error(void)
{
	perror("fork failed");
	g_errno = 1;
}
