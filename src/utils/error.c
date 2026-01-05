#include <stdio.h>

#include "../../libs/libft/libft.h"

void	print_error(const char *err)
{
    write(2, err, ft_strlen(err));
}
