#include <termios.h>

#include "../includes/utils.h"
#include "../includes/minishell.h"

extern void	rl_clear_history(void);

int	main(int argc __attribute__((unused)),
		char **argv __attribute__((unused)), char **envp)
{
	struct termios		original_termios;

	set_term_config(&original_termios);
	psig_set();
	interactive_loop(envp);
	restore_terminal_settings(&original_termios);
	return (0);
}
