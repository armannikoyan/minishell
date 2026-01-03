#include "../../libs/libft/libft.h"

void	move_cursor_up(void)
{
	ft_printf("\033[1A");
	ft_printf("\033[2K");
	ft_printf("\033[G");
}
