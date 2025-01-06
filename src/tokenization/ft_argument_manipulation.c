/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_argument_manipulation.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:34:14 by anikoyan          #+#    #+#             */
/*   Updated: 2025/01/06 18:27:30 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_starting_quote(char c)
{
	return (c == '\'' || c == '\"');
}

static char	*process_quote_remove(const char *content, char starting_quote)
{
	size_t	i;
	size_t	j;
	char	*result;

	result = malloc(ft_strlen(content) + 1);
	if (!result)
	{
		perror("Memory allocation failed");
		exit(EXIT_FAILURE);
	}
	i = 1;
	j = 0;
	while (content[i])
	{
		if (content[i] == starting_quote)
		{
			i++;
			continue ;
		}
		result[j++] = content[i++];
	}
	result[j] = '\0';
	return (result);
}

void	ft_quote_removal(t_token *token)
{
	char	starting_quote;
	char	*processed_content;

	if (!token || !token->content)
		return ;
	starting_quote = token->content[0];
	if (!is_starting_quote(starting_quote))
		return ;
	processed_content = process_quote_remove(token->content, starting_quote);
	free(token->content);
	token->content = processed_content;
}

DIR	*ft_open_directory(const char *path, struct dirent **entry)
{
	DIR	*dir;

	if (ft_strcmp(path, "") == 0)
		dir = opendir("./");
	else
		dir = opendir(path);
	if (!dir)
	{
		ft_report_error("no matches found: ", path, 1);
		return (NULL);
	}
	*entry = readdir(dir);
	if (!*entry)
	{
		ft_report_error("no matches found: ", path, 1);
		closedir(dir);
		return (NULL);
	}
	return (dir);
}

bool	ft_is_matching_pattern(const char *entry_name,
		const char *prefix, const char *postfix, bool *matches_found)
{
	if ((ft_strncmp(entry_name, prefix, ft_strlen(prefix)) == 0)
		&& (ft_strlen(entry_name)
			>= ft_strlen(prefix) + ft_strlen(postfix))
		&& (ft_strcmp(entry_name + ft_strlen(entry_name)
				- ft_strlen(postfix), postfix) == 0))
	{
		*matches_found = true;
		return (true);
	}
	return (false);
}

void	ft_insert_new_node(t_list **lst, t_list *new_node)
{
	if (*lst)
	{
		new_node->next = (*lst)->next;
		(*lst)->next = new_node;
		*lst = (*lst)->next;
	}
	else
		*lst = new_node;
}

bool	ft_handle_no_matches_found(t_list *lst, DIR *dir)
{
	ft_report_error("no matches found: ",
		((t_token *)lst->content)->content, 1);
	ft_close_directory(dir);
	return (false);
}

bool	ft_skip_entry(struct dirent *entry, const char *prefix)
{
	if ((entry->d_name[0] == '.' && ((ft_strlen(entry->d_name) == 1)
				|| (ft_strlen(entry->d_name) == 2
					&& entry->d_name[1] == '.')))
		|| (ft_strncmp(prefix, ".", 1) != 0
			&& ft_strncmp(entry->d_name, ".", 1) == 0))
		return (true);
	return (false);
}

void	ft_create_and_insert_new_node(t_list **lst,
		const char *path, const char *entry_name)
{
	t_token	*new_token;
	t_list	*new_node;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		exit(EXIT_FAILURE);
	new_token->content = ft_strjoin(path, entry_name);
	if (!new_token->content)
		exit(EXIT_FAILURE);
	new_node = ft_lstnew(new_token);
	if (!new_node)
		exit(EXIT_FAILURE);
	ft_insert_new_node(lst, new_node);
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

bool	ft_close_directory(DIR *dir)
{
	if (closedir(dir) == -1)
		ft_report_error("closedir failed: ", NULL, 1);
	return (true);
}

unsigned short	ft_path_len(char *str)
{
	unsigned short	i;

	i = 0;
	while (str[i] && str[i] != '*')
		i++;
	while (str[i] && &str[i] != str && str[i] != '/')
		i--;
	if (str[i] == '/')
		i++;
	if (&str[i] == str)
		return (0);
	return (&str[i] - str);
}

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

bool	ft_is_pattern_match(t_token *token)
{
	return (token && token->content && ft_strchr(token->content, '*')
		&& token->content[0] != '\'' && token->content[0] != '\"');
}

void	ft_free_resources(char *dir_name, char *prefix, char *postfix)
{
	free(dir_name);
	free(prefix);
	free(postfix);
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

bool	ft_process_path_patterns(t_list **lst_ref)
{
	t_list	*current ;
	t_list	*prev;
	t_list	*next;
	t_token	*token;

	current = *lst_ref;
	prev = NULL;
	while (current)
	{
		next = current->next;
		token = (t_token *)current->content;
		if (ft_is_pattern_match(token))
		{
			if (!ft_process_pattern(lst_ref, current, prev, token))
				return (false);
		}
		else
		{
			ft_quote_removal(token);
			prev = current;
		}
		current = next;
	}
	return (true);
}
