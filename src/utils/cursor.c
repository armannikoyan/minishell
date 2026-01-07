#include <stdio.h>

void	move_cursor_up(void)
{
	printf("\033[1A");
	printf("\033[2K");
	printf("\033[G");
}
