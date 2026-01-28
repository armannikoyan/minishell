/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 23:48:01 by lvarnach          #+#    #+#             */
/*   Updated: 2024/04/09 18:29:34 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static size_t	ft_count_words(const char *str, char c)
{
	size_t	count;

	count = 0;
	while (*str)
	{
		while (*str == c)
			str++;
		if (*str)
			count++;
		while (*str && *str != c)
			str++;
	}
	return (count);
}

static char	*ft_extract_word(const char *str, char c, size_t *pos)
{
	size_t	start;
	size_t	end;

	start = *pos;
	while (str[start] && str[start] == c)
		start++;
	end = start;
	while (str[end] && str[end] != c)
		end++;
	*pos = end;
	return (ft_substr(str, start, end - start));
}

static void	ft_strsfree(char **strs, size_t i)
{
	while (i)
		free(strs[--i]);
	free(strs);
}

char	**ft_split(char const *s, char c)
{
	size_t	num_words;
	size_t	i;
	size_t	pos;
	char	**strs;

	if (!s)
		return (NULL);
	num_words = ft_count_words(s, c);
	strs = malloc(sizeof(char *) * (num_words + 1));
	if (!strs)
		return (NULL);
	i = 0;
	pos = 0;
	while (i < num_words)
	{
		strs[i] = ft_extract_word(s, c, &pos);
		if (!strs[i])
		{
			ft_strsfree(strs, i);
			return (NULL);
		}
		i++;
	}
	strs[i] = NULL;
	return (strs);
}
