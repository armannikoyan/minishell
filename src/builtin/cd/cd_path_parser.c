/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_path_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 02:03:30 by lvarnach          #+#    #+#             */
/*   Updated: 2026/01/27 02:03:31 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "../../../libs/libft/libft.h"
#include "cd_path_utils.h"
#include "utils.h"

// Frees an array of strings produced by split_env_var.
// Safe to call on fully or partially constructed arrays.
void	free_split(char **paths)
{
	size_t	i;

	i = 0;
	if (!paths)
		return ;
	while (paths[i])
	{
		free(paths[i]);
		i++;
	}
	free(paths);
}

// Counts the number of segments in the colon-separated string.
// Every ':' implies a split, so segments = colons + 1.
static size_t	count_segments(const char *str)
{
	size_t	count;

	count = 1;
	while (*str)
	{
		if (*str == ':')
			count++;
		str++;
	}
	return (count);
}

// Creates a new string from the range [start, end).
// If the length is 0 (empty segment), allocates and returns ".".
static char	*extract_segment(const char *start, const char *end)
{
	char	*segment;
	size_t	len;

	len = end - start;
	if (len == 0)
		segment = ft_strdup(".");
	else
	{
		segment = malloc(len + 1);
		if (segment)
		{
			ft_memcpy(segment, start, len);
			segment[len] = '\0';
		}
	}
	return (segment);
}

// Iterates through the string and fills the allocated array.
// Returns true on success, false on allocation failure.
static bool	fill_split_array(char **result, const char *str)
{
	char	*start;
	size_t	i;

	start = (char *)str;
	i = 0;
	while (*str)
	{
		if (*str == ':')
		{
			result[i] = extract_segment(start, str);
			if (!result[i])
				return (false);
			i++;
			start = (char *)str + 1;
		}
		str++;
	}
	result[i] = extract_segment(start, str);
	if (!result[i])
		return (false);
	return (true);
}

// Splits a colon-separated environment variable (like CDPATH) into an array.
//
// Behavior:
// - Uses ':' as a delimiter.
// - Empty segments (start, end, or "::") are converted to ".".
// - Returns a NULL-terminated array of strings.
// - Returns NULL if str is NULL, empty, or on malloc failure.
char	**split_ev(const char *str, char ***result)
{
	if (!str || !*str)
		return (NULL);
	*result = ft_calloc(count_segments(str) + 1, sizeof(char *));
	if (!result)
	{
		print_error("minishell: split_env_var: malloc", false);
		return (NULL);
	}
	if (!fill_split_array(*result, str))
	{
		print_error("minishell: split_env_var: malloc", false);
		free_split(*result);
		return (NULL);
	}
	return (*result);
}
