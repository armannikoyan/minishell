/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 23:35:11 by anikoyan          #+#    #+#             */
/*   Updated: 2025/01/15 17:27:47 by gsimonia         ###   ########.fr       */
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

int	handle_sign_and_skip_whitespace(const char *str, int *sign)
{
	long long	i;

	i = 0;
	*sign = 1;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\r'
		|| str[i] == '\v' || str[i] == '\f' || str[i] == '\n')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			*sign = -1;
		i++;
	}
	return (i);
}

int	check_for_overflow(const char *str, long long *result, int sign)
{
	long long	i;

	i = 0;
	*result = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (*result > LLONG_MAX / 10)
			return (1);
		if (*result == LLONG_MAX / 10)
		{
			if ((sign == 1 && (str[i] - '0') > LLONG_MAX % 10)
				|| (sign == -1 && (str[i] - '0') > -(LLONG_MIN % 10)))
				return (1);
		}
		*result = (*result * 10) + (str[i] - '0');
		i++;
	}
	return (0);
}
