#include <signal.h>
#include <stdio.h>
#include <readline/readline.h>

#include "../../libs/libft/libft.h"

extern void	rl_replace_line(const char *text, int clear_undo);

void	psig_handler(int signo)
{
	if (signo == SIGINT)
	{
		ft_printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	else if (signo == SIGQUIT)
		return ;
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
