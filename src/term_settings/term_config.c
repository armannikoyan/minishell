#include <termios.h>
#include <unistd.h>

void	restore_terminal_settings(struct termios *original)
{
	tcsetattr(STDIN_FILENO, TCSANOW, original);
}

void	set_term_config(struct termios *original)
{
	struct termios	new_termios;

	tcgetattr(STDIN_FILENO, original);
	new_termios = *original;
	new_termios.c_lflag &= ~ECHOCTL;
	new_termios.c_cc[VMIN] = 1;
	new_termios.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
}
