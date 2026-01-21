#include <termios.h>
#include <unistd.h>

#include "utils.h"

void	restore_terminal_settings(struct termios *original)
{
	tcsetattr(STDIN_FILENO, TCSANOW, original);
}

void	set_term_config(struct termios *original)
{
	struct termios	new_termios;

	if (tcgetattr(STDIN_FILENO, original) == -1)
		print_error("minishell: tcgetattr", false);
	new_termios = *original;
	new_termios.c_lflag &= ~(ECHOCTL);
	new_termios.c_cc[VMIN] = 1;
	new_termios.c_cc[VTIME] = 0;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &new_termios) == -1)
		print_error("minishell: tcsetattr", false);
}
