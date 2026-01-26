/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 00:58:54 by lvarnach          #+#    #+#             */
/*   Updated: 2026/01/27 01:08:49 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/stat.h>

#include "path_utils.h"
#include "utils.h"
#include "../../libs/libft/libft.h"

// Concatenates two path components into a single path.
// Automatically inserts a '/' between components if needed.
// Returns a newly allocated string which must be freed by the caller.
char	*concat_path(const char *p1, const char *p2)
{
	size_t		len1;
	size_t		len2;
	char		*full;

	if (p1 == NULL || p2 == NULL)
		return (NULL);
	len1 = ft_strlen(p1);
	len2 = ft_strlen(p2);
	full = malloc(len1 + 1 + len2 + 1);
	if (full == NULL)
	{
		print_error("concat_path: malloc", false);
		return (NULL);
	}
	ft_memcpy(full, p1, len1);
	if (len1 > 0 && p1[len1 - 1] != '/')
		full[len1++] = '/';
	ft_memcpy(full + len1, p2, len2);
	full[len1 + len2] = '\0';
	return (full);
}

// Removes the last directory component from an absolute path.
// The string is modified in place.
// If the path is "/", it is left unchanged.
char	*remove_last_dir(char *path)
{
	char	*p;

	if (!path || path[0] != '/')
		return (path);
	if (path[1] == '\0')
		return (path);
	p = path;
	while (*p)
		p++;
	while (p > path && *p != '/')
		p--;
	if (p == path)
		path[1] = '\0';
	else
		*p = '\0';
	return (path);
}

// Expands the '~' symbol using the HOME variable from
// the environment hash table.
// Supports only '~' and '~/...' forms.
// Expansion of '~user' is intentionally not supported
// due to project restrictions.
// Returns a newly allocated string or NULL on error.
char	*expand_tilda(const char *path, t_hash_table *ht)
{
	const char	*home;
	const char	*rest;
	t_entry		*entry;

	if (path == NULL)
		return (NULL);
	if (path[1] == '/')
		rest = path + 2;
	else
		return (NULL);
	entry = ht_get(ht, "HOME");
	if (entry == NULL)
	{
		print_error("expand_tilda: environment variable ''HOME'' doesn't exist",
			true);
		return (NULL);
	}
	home = entry->val;
	if (home == NULL)
		home = "";
	return (concat_path(home, rest));
}

// Checks filesystem properties of a given path and returns the result
// as a bitmask.
//
// The function verifies existence, determines file type (directory or
// regular file), and checks access permissions (read, write, execute).
//
// Returned value is a bitwise OR of FS_* flags defined in utils.h.
// The caller is responsible for interpreting the result.
int	check_access(const char *path)
{
	struct stat	st;
	size_t		result;

	result = 0;
	if (access(path, F_OK) != 0)
		return (result);
	result |= EXISTS_BIT;
	if (stat(path, &st) != 0)
	{
		result |= STAT_FAIL_BIT;
		return (result);
	}
	if (S_ISDIR(st.st_mode))
		result |= DIR_BIT;
	else if (S_ISREG(st.st_mode))
		result |= FILE_BIT;
	if (access(path, R_OK) == 0)
		result |= READ_BIT;
	if (access(path, W_OK) == 0)
		result |= WRITE_BIT;
	if (access(path, X_OK) == 0)
		result |= EXEC_BIT;
	return (result);
}
