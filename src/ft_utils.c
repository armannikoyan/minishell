#include "../includes/minishell.h"

static char	*ft_subwdir(char *abs_wdir)
{
	unsigned int	len;
	size_t			i;

	i = 0;
	while (abs_wdir[i])
		i++;
	len = 0;
	while (abs_wdir[i - 1] != '/' && i > 0)
	{
		i--;
		len++;
	}
	if (len == 0)
		return (ft_strdup("/"));
	return (ft_substr(abs_wdir, i, len));
}

void	ft_entry_info_helper(char **username, char **w_dir)
{
	char	*abs_wdir;

	abs_wdir = NULL;
	abs_wdir = getcwd(abs_wdir, MAXPATHLEN);
	*username = ft_strdup(getenv("LOGNAME"));
	*w_dir = ft_subwdir(abs_wdir);
	free(abs_wdir);
}
