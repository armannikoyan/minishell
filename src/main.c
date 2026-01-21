#include <termios.h>

#include "term_settings.h"
#include "minishell.h"

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
