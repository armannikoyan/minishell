/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_argument_manipulation.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:34:14 by anikoyan          #+#    #+#             */
/*   Updated: 2024/12/22 23:51:05 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_quote_removal(t_token *token)
{
	char			*new_content;
	unsigned short	len;

	if ((token->content[0] == '\'' || token->content[0] == '\"')
		&& token->content[0] == token->content[ft_strlen(token->content) - 1])
	{
		len = ft_strlen(token->content);
		new_content = ft_substr(token->content, 1, len - 2);
		free(token->content);
		token->content = new_content;
	}
}

bool	ft_list_files_in_directory_with_pattern(const char *path,
	t_list **list_ref, const char *prefix, const char *postfix)
{
	DIR				*dir;
	struct dirent	*entry;
	t_token			*new_token;
	t_list			*new_node;
	t_list			*current;
	bool			matches_found;

	if (ft_strcmp(path, "") == 0)
		dir = opendir("./");
	else
		dir = opendir(path);
	if (!dir)
	{
		ft_report_error("no matches found: ", ((t_token *)(*list_ref)->next->content)->content, 1);
		return (false);
	}
	entry = readdir(dir);
	while (entry != NULL)
	{
		if ((entry->d_name[0] == '.' && ((ft_strlen(entry->d_name) == 1)
					|| (ft_strlen(entry->d_name) == 2
						&& entry->d_name[1] == '.')))
			|| (ft_strncmp(prefix, ".", 1) != 0
				&& ft_strncmp(entry->d_name, ".", 1) == 0))
		{
			entry = readdir(dir);
			continue ;
		}
		if ((ft_strncmp(entry->d_name, prefix, ft_strlen(prefix)) == 0)
			&& (ft_strlen(entry->d_name)
				>= ft_strlen(prefix) + ft_strlen(postfix))
			&& (ft_strcmp(entry->d_name + ft_strlen(entry->d_name)
					- ft_strlen(postfix), postfix) == 0))
		{
			matches_found = true;
			new_token = (t_token *)malloc(sizeof(t_token));
			if (!new_token)
				exit(EXIT_FAILURE);
			new_token->content = ft_strjoin(path, entry->d_name);
			if (!new_token->content)
				exit(EXIT_FAILURE);
			new_node = ft_lstnew(new_token);
			if (!new_node)
				exit(EXIT_FAILURE);
			current = *list_ref;
			if (!current)
				*list_ref = new_node;
			else
			{
				while (current->next)
					current = current->next;
				current->next = new_node;
			}
		}
		if (!matches_found)
		{
			ft_report_error("no matches found: ", ((t_token *)(*list_ref)->next->content)->content, 1);
			return (false);
		}
		entry = readdir(dir);
	}
	if (closedir(dir) == -1)
		perror("closedir failed");
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

bool	ft_process_path_patterns(t_list **lst_ref)
{
	t_list	*current;
	t_list	*prev;
	t_list	*next;
	t_token	*token;
	char	*dir_name;
	char	*prefix;
	char	*postfix;

	current = *lst_ref;
	prev = NULL;
	prefix = NULL;
	postfix = NULL;
	while (current)
	{
		next = current->next;
		token = (t_token *)current->content;
		if (token && token->content && ft_strchr(token->content, '*')
			&& token->content[0] != '\'' && token->content[0] != '\"')
		{
			ft_extract_pattern(token->content, &prefix, &postfix);
			dir_name = ft_get_dir_name(token->content);
			if (ft_list_files_in_directory_with_pattern(dir_name,
				lst_ref, prefix, postfix))
			{
				free(dir_name);
				free(prefix);
				free(postfix);
				if (prev)
					prev->next = current->next;
				else
					*lst_ref = current->next;
				ft_tokendelone(current);
				current = next;
			}
			else
			{
				free(dir_name);
				free(prefix);
				free(postfix);
				return (false);
			}
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
