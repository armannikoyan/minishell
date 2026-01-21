#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>

void	psig_handler(int signo)
{
	if (signo == SIGINT)
	{
		rl_replace_line("^C\n", 0);
		rl_redisplay();
		write(STDOUT_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	else if (signo == SIGQUIT)
		rl_redisplay();
}

void	psig_set(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = psig_handler;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}
