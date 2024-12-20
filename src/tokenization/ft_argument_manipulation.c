/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_argument_manipulation.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:34:14 by anikoyan          #+#    #+#             */
/*   Updated: 2024/12/20 23:36:14 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_quote_romoval(t_token *token)
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

void	ft_list_files_in_directory(const char *path, const char *pattern, t_list **list_ref)
{
	DIR				*dir;
	struct dirent	*entry;
	t_token			*new_token;
	t_list			*new_node;
	t_list			*current;

	dir = opendir(path);
	if (!dir)
	{
		perror("opendir failed");
		return ;
	}
	while (entry != NULL)
	{
		if (entry->d_name[0] == '.' && ((ft_strlen(entry->d_name) == 1)
				|| (ft_strlen(entry->d_name) == 2 && entry->d_name[1] == '.')))
			continue ;
		new_token = (t_token *)malloc(sizeof(t_token));
		if (!new_token)
		{
			perror("malloc failed");
			closedir(dir);
			return ;
		}
		new_token->content = ft_strjoin(pattern, entry->d_name);
		if (!new_token->content)
		{
			perror("ft_strdup failed");
			free(new_token);
			closedir(dir);
			return ;
		}
		new_node = ft_lstnew(new_token);
		if (!new_node)
		{
			perror("ft_lstnew failed");
			free(new_token->content);
			free(new_token);
			closedir(dir);
			return ;
		}
		current = *list_ref;
		if (!current)
			*list_ref = new_node;
		else
		{
			while (current->next)
				current = current->next;
			current->next = new_node;
		}
		entry = readdir(dir);
		// TODO: make pattern matching
	}
	if (closedir(dir) == -1)
		perror("closedir failed");
}

unsigned short	ft_path_len(char *str)
{
	unsigned short	i;

	i = 0;
	while (str[i] && str[i] != '*')
		i++;
	while (str[i] && &str[i] != str && str[i] != '/')
		i--;
	if (&str[i] == str)
		return (0);
	return (&str[i] - str + 1);
}

char	*ft_get_pattern(char *str)
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
		path = ft_strdup("./");
		if (!path)
			exit(EXIT_FAILURE);
	}
	return (path);
}

void	ft_process_path_patterns(t_list **lst_ref)
{
	t_list	*current;
	t_list	*prev;
	t_list	*next;
	t_token	*token;
	char	*dir_name;
	char	*prefix;

	current = *lst_ref;
	prev = NULL;
	while (current)
	{
		next = current->next;
		token = (t_token *)current->content;
		if (token && token->content && ft_strchr(token->content, '*'))
		{
			dir_name = ft_get_dir_name(token->content);
			prefix = ft_get_pattern(token->content);
			ft_list_files_in_directory(dir_name, prefix, lst_ref);
			free(dir_name);
			free(prefix);
			if (prev)
				prev->next = current->next;
			else
				*lst_ref = current->next;
			ft_tokendelone(current);
			current = next;
		}
		else
		{
			ft_quote_romoval(token);
			prev = current;
		}
		current = next;
	}
}
