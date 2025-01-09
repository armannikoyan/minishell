#include "../../includes/minishell.h"

char	*ft_get_dir_name(char *str)
{
	char			*path;
	unsigned short	len;

	len = ft_path_len(str);
	if (len)
	{
		path = (char *)malloc(sizeof(char) * (len + 1));
		if (!path)
			exit(EXIT_FAILURE);
		ft_strlcpy(path, str, len + 1);
	}
	else
	{
		path = ft_strdup("");
		if (!path)
			exit(EXIT_FAILURE);
	}
	return (path);
}

bool	ft_list_files_in_directory_with_pattern(const char *path, t_list *lst,
		const char *prefix, const char *postfix)
{
	DIR				*dir;
	struct dirent	*entry;
	bool			mf;

	mf = false;
	dir = ft_open_directory(path, &entry);
	if (!dir)
		return (false);
	while (entry != NULL)
	{
		if (!ft_skip_entry(entry, prefix))
			if (ft_is_matching_pattern(entry->d_name, prefix, postfix, &mf))
				ft_create_and_insert_new_node(&lst, path, entry->d_name);
		entry = readdir(dir);
	}
	if (!mf)
		return (ft_handle_no_matches_found(lst, dir));
	return (ft_close_directory(dir));
}

void	ft_free_resources(char *dir_name, char *prefix, char *postfix)
{
	free(dir_name);
	free(prefix);
	free(postfix);
}

void	ft_extract_pattern(const char *pattern, char **prefix, char **postfix)
{
	const char	*star;
	const char	*slash;
	const char	*last_slash;

	*prefix = NULL;
	*postfix = NULL;
	star = ft_strchr(pattern, '*');
	if (!star)
		return ;
	last_slash = star;
	while (last_slash > pattern && *(last_slash - 1) != '/')
		last_slash--;
	*prefix = ft_substr(last_slash, 0, star - last_slash);
	if (!*prefix)
		exit(EXIT_FAILURE);
	slash = ft_strchr(star + 1, '/');
	if (slash)
		*postfix = ft_substr(star + 1, 0, slash - (star + 1));
	else
		*postfix = ft_strdup(star + 1);
	if (!*postfix)
	{
		free(*prefix);
		exit(EXIT_FAILURE);
	}
}

bool	ft_process_pattern(t_list **lst_ref,
		t_list *current, t_list *prev, t_token *token)
{
	char	*dir_name;
	char	*prefix;
	char	*postfix;

	prefix = NULL;
	postfix = NULL;
	ft_extract_pattern(token->content, &prefix, &postfix);
	dir_name = ft_get_dir_name(token->content);
	if (ft_list_files_in_directory_with_pattern(dir_name,
			current, prefix, postfix))
	{
		ft_free_resources(dir_name, prefix, postfix);
		if (prev)
		{
			prev->next = current->next;
			prev = prev->next;
		}
		else
			*lst_ref = current->next;
		ft_tokendelone(current);
		current = prev->next;
		return (true);
	}
	ft_free_resources(dir_name, prefix, postfix);
	return (false);
}
