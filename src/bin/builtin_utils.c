/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 23:35:11 by anikoyan          #+#    #+#             */
/*   Updated: 2025/01/06 18:24:02 by anikoyan         ###   ########.fr       */
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

long long	ft_long_atoi(const char *str)
{
	long long	i;
	long long	j;
	long long	min;

	i = 0;
	j = 0;
	min = 1;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\r'
		|| str[i] == '\v' || str[i] == '\f' || str[i] == '\n')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			min = -1;
		i++;
	}
	if (str[i] == '\0')
		return (0);
	while (str[i] >= '0' && str[i] <= '9')
	{
		j *= 10;
		j += (str[i] - '0');
		i++;
	}
	return (j * min);
}
