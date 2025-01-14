/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 23:35:11 by anikoyan          #+#    #+#             */
/*   Updated: 2025/01/14 14:21:37 by gsimonia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_valid_env_var_key(const char *str)
{
	int	i;

	if (!str || !(*str) || (*str == '='))
		return (0);
	i = 0;
	while (str[i] && str[i] != '=')
	{
		if (!(str[i] == '_' || (str[i] >= 'a' && str[i] <= 'z')
				|| (str[i] >= 'A' && str[i] <= 'Z')
				|| (i > 0 && str[i] >= '0' && str[i] <= '9')))
			return (0);
		i++;
	}
	return (1);
}

static int	ft_isdelimiter(char c, const char *delimiters)
{
	while (*delimiters)
	{
		if (c == *delimiters)
			return (1);
		delimiters++;
	}
	return (0);
}

char	*ft_strtok(char *str, const char *delimiters, char **save_ptr)
{
	char	*token;

	if (str != NULL)
		*save_ptr = str;
	if (*save_ptr == NULL)
		return (NULL);
	while (**save_ptr && ft_isdelimiter(**save_ptr, delimiters))
		(*save_ptr)++;
	if (**save_ptr == '\0')
		return (NULL);
	token = *save_ptr;
	while (**save_ptr && !ft_isdelimiter(**save_ptr, delimiters))
		(*save_ptr)++;
	if (**save_ptr != '\0')
	{
		**save_ptr = '\0';
		(*save_ptr)++;
	}
	return (token);
}
