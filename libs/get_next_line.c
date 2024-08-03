/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasargsy <dasargsy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 16:42:14 by dasargsy          #+#    #+#             */
/*   Updated: 2024/05/25 11:36:14 by dasargsy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/gnl.h"

long long	find_newline(char *readed)
{
	long long	i;

	i = 0;
	if (!readed)
		return (-1);
	while (readed[i])
	{
		if (readed[i] == '\n')
			return (i);
		i++;
	}
	return (-1);
}

char	*get_st_char(char *readed, char *s)
{
	char		*st_char;

	if (readed == NULL)
	{
		if (find_newline(s) == -1 || !s)
		{
			free(s);
			return (NULL);
		}
		st_char = ft_gsubstr(s, find_newline(s) + 1,
				ft_gstrlen(s) - find_newline(s), 1);
		return (st_char);
	}
	if (s != NULL)
		st_char = ft_gstrjoin(s, readed, 1, 1);
	else
		st_char = ft_gsubstr(readed, 0, ft_gstrlen(readed), 1);
	if (find_newline(st_char) == -1 || !st_char)
	{
		free(st_char);
		return (NULL);
	}
	st_char = ft_gsubstr(st_char, find_newline(st_char) + 1,
			ft_gstrlen(st_char) - find_newline(st_char), 1);
	return (st_char);
}

char	*get_return_line(char *st_char, char *readed)
{
	char		*line;
	long long	index;

	if (readed == NULL && st_char == NULL)
		return (NULL);
	else if (readed == NULL && st_char != NULL)
	{
		index = find_newline(st_char);
		if (index == -1)
			line = ft_gsubstr(st_char, 0, ft_gstrlen(st_char), 0);
		else
			line = ft_gsubstr(st_char, 0, index + 1, 0);
		return (line);
	}
	if (st_char != NULL)
		line = ft_gstrjoin(st_char, readed, 0, 0);
	else
		line = ft_gsubstr(readed, 0, ft_gstrlen(readed), 0);
	index = find_newline(line);
	if (index == -1 || !line)
		index = ft_gstrlen(line) - 1;
	line = ft_gsubstr(line, 0, index + 1, 1);
	return (line);
}

char	*get_readed(int fd, char **st_char)
{
	char		*readed;
	char		*temp;
	long long	size;

	readed = (char *)ft_gcalloc(1, BUFFER_SIZE + 1);
	size = read(fd, readed, BUFFER_SIZE);
	if (size < 0)
	{
		free(*st_char);
		*st_char = NULL;
	}
	if (size <= 0 || !readed)
	{
		free(readed);
		return (NULL);
	}
	while (find_newline(readed) == -1 && size != 0)
	{
		temp = (char *)ft_gcalloc(1, BUFFER_SIZE + 1);
		size = read(fd, temp, BUFFER_SIZE);
		readed = ft_gstrjoin(readed, temp, 1, 1);
	}
	return (readed);
}

char	*get_next_line(int fd)
{
	static char	*st_char;
	char		*line;
	char		*readed;

	if (fd < 0 || BUFFER_SIZE <= 0 || fd > OPEN_MAX)
	{
		free(st_char);
		st_char = NULL;
		return (NULL);
	}
	readed = get_readed(fd, &st_char);
	line = get_return_line(st_char, readed);
	st_char = get_st_char(readed, st_char);
	return (line);
}
