/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenization.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 20:33:00 by anikoyan          #+#    #+#             */
/*   Updated: 2024/12/13 12:59:10 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// static bool	ft_identify_command(t_token **token, char **path)
// {
// 	char	*line;
// 	char	*tmp;
// 	int		i;
// 	bool	result;

// 	result = false;
// 	if (access((*token)->content, X_OK) == 0)
// 		result = true;
// 	i = 0;
// 	while (path[i] && !result)
// 	{
// 		tmp = ft_strjoin("/", (*token)->content);
// 		if (!tmp)
// 			exit(EXIT_FAILURE);
// 		line = ft_strjoin(path[i], tmp);
// 		free(tmp);
// 		if (!line)
// 			exit(EXIT_FAILURE);
// 		if (access(line, X_OK) == 0)
// 		{
// 			free((*token)->content);
// 			(*token)->content = ft_strdup(line);
// 			result = true;
// 		}
// 		free(line);
// 		i++;
// 	}
// 	ft_free_2d_array((void ***)&path);
// 	return (result);
// }

// static t_token	*ft_build_token(char *input, unsigned short *index)
// {
// 	t_token			*token;
// 	unsigned short	len;
// 	unsigned short	i;

// 	token = (t_token *)malloc(sizeof(t_token));
// 	if (!token)
// 		exit(EXIT_FAILURE);
// 	i = 0;
// 	len = 0;
// 	while (input[*index + i] && input[*index + i] != ' ')
// 	{
// 		if (input[*index + i] == '\"' || input[*index + i] == '\'')
// 		{
// 			len++;
// 			i++;
// 			while (input[*index + i] && input[*index + i] != '\"'
// 				&& input[*index + i] != '\'')
// 			{
// 				len++;
// 				i++;
// 			}
// 		}
// 		len++;
// 		i++;
// 	}
// 	token->content = (char *)malloc(sizeof(char) * (len + 1));
// 	if (!token->content)
// 		exit(EXIT_FAILURE);
// 	i = 0;
// 	while (input[*index + i] && input[*index + i] != ' ')
// 	{
// 		if (input[*index + i] == '\"' || input[*index + i] == '\'')
// 		{
// 			token->content[i] = input[*index + i];
// 			i++;
// 			while (input[*index + i] && input[*index + i] != '\"'
// 				&& input[*index + i] != '\'')
// 			{
// 				token->content[i] = input[*index + i];
// 				i++;
// 			}
// 		}
// 		token->content[i] = input[*index + i];
// 		i++;
// 	}
// 	token->content[i++] = '\0';
// 	*index += len;
// 	while (input[*index] && input[*index] == ' ')
// 		(*index)++;
// 	token->type = 0;
// 	return (token);
// }

// static void	ft_assign_token_type(t_list ***lst)
// {
// 	struct stat	statbuf;
// 	t_list		*tmp;
// 	t_token		*token;

// 	tmp = **lst;
// 	while (tmp)
// 	{
// 		token = (t_token *)tmp->content;
// 		if (ft_identify_command(&token, ft_split(getenv("PATH"), ':')))
// 		{
// 			if (stat(token->content, &statbuf) == 0 && !S_ISDIR(statbuf.st_mode)
// 				&& (statbuf.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)))
// 			{
// 				token->type = 'X';
// 				while (tmp && tmp->next
// 					&& !ft_isoperator(((t_token *)tmp->next->content)->content))
// 				{
// 					((t_token *)tmp->next->content)->type = 'A';
// 					tmp = tmp->next;
// 				}
// 			}
// 			else
// 				token->type = 'E';
// 		}
// 		else if (access(token->content, F_OK) == 0)
// 			token->type = 'F';
// 		else if (ft_isoperator(token->content))
// 		{
// 			token->type = 'O';
// 			if ((token->content[0] == '>' || token->content[0] == '<')
// 				&& tmp->next && (t_token *)tmp->next->content)
// 			{
// 				tmp = tmp->next;
// 				((t_token *)tmp->content)->type = 'F';
// 				tmp = tmp->next;
// 				while (tmp && (t_token *)tmp->content
// 					&& ((t_token *)tmp->content)->content
// 					&& !ft_isoperator(((t_token *)tmp->content)->content))
// 				{
// 					((t_token *)tmp->content)->type = 'A';
// 					if (ft_isoperator(((t_token *)tmp->next->content)->content))
// 						break ;
// 					tmp = tmp->next;
// 				}
// 			}
// 		}
// 		else
// 			token->type = 'E';
// 		if (tmp)
// 			tmp = tmp->next;
// 	}
// }

void	copy_token_content(char *dest, const char *input, unsigned short *index)
{
	unsigned short	i;

	i = 0;
	while (input[*index + i] && input[*index + i] != ' ')
	{
		if (input[*index + i] == '\"' || input[*index + i] == '\'')
		{
			dest[i] = input[*index + i++];
			while (input[*index + i]
				&& input[*index + i] != '\"' && input[*index + i] != '\'')
				dest[i] = input[*index + i++];
		}
		dest[i] = input[*index + i];
		i++;
	}
	dest[i] = '\0';
}

static	t_token	*ft_build_token(char *input, unsigned short *index)
{
	t_token			*token;
	unsigned short	len;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		exit(EXIT_FAILURE);
	len = calculate_token_length(input, index);
	token->content = (char *)malloc(sizeof(char) * (len + 1));
	if (!token->content)
		exit(EXIT_FAILURE);
	copy_token_content(token->content, input, index);
	*index += len;
	while (input[*index] && input[*index] == ' ')
		(*index)++;
	token->type = 0;
	return (token);
}

void	handle_operator_token(t_list **tmp)
{
	t_token	*token;

	token = (t_token *)(*tmp)->content;
	if ((token->content[0] == '>' || token->content[0] == '<') && (*tmp)->next)
	{
		(*tmp) = (*tmp)->next;
		((t_token *)(*tmp)->content)->type = 'F';
		while ((*tmp)->next
			&& !ft_isoperator(((t_token *)(*tmp)->next->content)->content))
		{
			(*tmp) = (*tmp)->next;
			((t_token *)(*tmp)->content)->type = 'A';
		}
	}
}

static void	ft_assign_token_type(t_list ***lst)
{
	t_list	*tmp;
	t_token	*token;

	tmp = **lst;
	while (tmp)
	{
		token = (t_token *)tmp->content;
		if (ft_identify_command(&token, ft_split(getenv("PATH"), ':')))
			set_command_type(token);
		else if (access(token->content, F_OK) == 0)
			token->type = 'F';
		else if (ft_isoperator(token->content))
			handle_operator_token(&tmp);
		else
			token->type = 'E';
		if (tmp)
			tmp = tmp->next;
	}
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
	return (lst);
}
