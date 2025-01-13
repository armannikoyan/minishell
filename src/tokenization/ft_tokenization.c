/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenization.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 20:33:00 by anikoyan          #+#    #+#             */
/*   Updated: 2025/01/14 00:15:11 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	ft_try_command_path(t_token **token, char *directory)
{
	char	*tmp;
	char	*full_path;
	bool	result;

	tmp = ft_strjoin("/", (*token)->content);
	if (!tmp)
		exit(EXIT_FAILURE);
	full_path = ft_strjoin(directory, tmp);
	free(tmp);
	if (!full_path)
		exit(EXIT_FAILURE);
	result = (access(full_path, X_OK) == 0);
	if (result)
	{
		free((*token)->content);
		(*token)->content = ft_strdup(full_path);
	}
	free(full_path);
	return (result);
}

bool	ft_identify_command(t_token **token, char **path)
{
	int		i;
	bool	result;

	result = (access((*token)->content, X_OK) == 0);
	i = 0;
	while (path[i] && !result)
	{
		result = ft_try_command_path(token, path[i]);
		i++;
	}
	ft_free_2d_array((void ***)&path);
	return (result);
}

static unsigned short	ft_calculate_length(char *input, unsigned short index)
{
	unsigned short	len;
	unsigned short	i;

	len = 0;
	i = 0;
	while (input[index + i] && input[index + i] != ' ')
	{
		if (input[index + i] == '\"' || input[index + i] == '\'')
		{
			len++;
			i++;
			while (input[index + i]
				&& input[index + i] != '\"' && input[index + i] != '\'')
			{
				len++;
				i++;
			}
		}
		len++;
		i++;
	}
	return (len);
}

static t_token	*ft_build_token(char *input, unsigned short *index)
{
	t_token			*token;
	unsigned short	len;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		exit(EXIT_FAILURE);
	token->type = 0;
	len = ft_calculate_length(input, *index);
	ft_fill_content(token, input, index, len);
	return (token);
}

t_list	**ft_tokenization(char *input)
{
	t_list			**lst;
	t_list			*tmp;
	t_token			*token;
	unsigned short	i;

	token = NULL;
	i = 0;
	lst = (t_list **)malloc(sizeof(t_list *));
	if (!lst)
		exit(EXIT_FAILURE);
	*lst = NULL;
	while (input[i])
	{
		token = ft_build_token(input, &i);
		tmp = (t_list *)malloc(sizeof(t_list));
		tmp->content = token;
		tmp->next = NULL;
		ft_lstadd_back(lst, tmp);
	}
	ft_assign_token_type(&lst);
	ft_process_path_patterns(lst);
	ft_assign_subshell_levels(lst);
	return (lst);
}
