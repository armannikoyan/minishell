#include <termios.h>
#include <unistd.h>

#include "term_settings.h"
#include "minishell.h"

int	main(int argc __attribute__((unused)),
		char **argv __attribute__((unused)), char **envp)
{
	struct termios		original_termios;
	int					is_tty;

	is_tty = isatty(STDIN_FILENO);
	if (is_tty) {
		set_term_config(&original_termios);
		psig_set();
	}
	interactive_loop(envp);
	if (is_tty)
		restore_terminal_settings(&original_termios);
	return (0);
}
