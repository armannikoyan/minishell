/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   formatters.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 20:36:38 by anikoyan          #+#    #+#             */
/*   Updated: 2024/07/29 21:25:20 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_mtx_free(char **mtx)
{
	int	i;

	i = 0;
	while (mtx[i])
	{
		free(mtx[i]);
		i++;
	}
	free(mtx);
}

char	**ft_format_argv(int argc, char **argv)
{
	char	**data;
	int		i;

	if (argc < 5)
		return (NULL);
	data = malloc(sizeof(char *) * argc);
	i = 0;
	while (argv[i + 1])
	{
		data[i] = ft_strdup(argv[i + 1]);
		if (!data[i])
		{
			ft_mtx_free(data);
			return (NULL);
		}
		i++;
	}
	data[i] = NULL;
	return (data);
}

char	**ft_get_envp(char **envp)
{
	int		i;
	char	**path;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5))
		i++;
	if (!envp[i])
		return (NULL);
	path = ft_split(envp[i] + 5, ':');
	return (path);
}

char	*ft_command_path(char *cmd, char **envp)
{
	char	*line;
	char	*tmp;
	int		i;

	if (access(cmd, X_OK) == 0)
		return (cmd);
	i = 0;
	while (envp[i])
	{
		tmp = ft_strjoin("/", cmd);
		if (!tmp)
			return (NULL);
		line = ft_strjoin(envp[i], tmp);
		if (!line)
			return (NULL);
		free(tmp);
		if (access(line, X_OK) == 0)
			return (line);
		free(line);
		i++;
	}
	return (NULL);
}
