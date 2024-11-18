/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_expansion_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 17:14:08 by anikoyan          #+#    #+#             */
/*   Updated: 2024/10/01 20:51:30 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	ft_is_valid_env(char *env)
{
	unsigned int	i;

	i = 0;
	if (!ft_isalpha(env[i]) && env[i] != '-')
		return (false);
	i++;
	while (env[i] && (!ft_isspace(env[i]) && env[i] != '\''
			&& env[i] != '"' && !ft_isoperator(&env[i])))
	{
		if (!ft_isalnum(env[i]) && env[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

unsigned int	ft_envlen(char *input, unsigned int i)
{
	unsigned int	len;

	len = 0;
	while (input[i] && (!ft_isspace(input[i]) && input[i] != '\''
			&& input[i] != '"' && !ft_isoperator(&input[i])))
	{
		len++;
		i++;
	}
	return (len);
}

unsigned short	ft_get_quote_count(char *input, unsigned short index)
{
	unsigned short	i;
	unsigned short	quote_count;

	i = 0;
	quote_count = 0;
	while (input[i] && i < index)
	{
		if (input[i] == '\'')
			quote_count++;
		i++;
	}
	return (quote_count);
}

unsigned short	ft_varlen(char *input)
{
	unsigned short	i;
	unsigned short	env_len;

	i = 0;
	env_len = 0;
	while (input[i])
	{
		if (input[i] && input[i] == '$' && ft_get_quote_count(input, i) % 2 == 0
			&& ft_is_valid_env(&(input[++i])))
			env_len++;
		i++;
	}
	return (env_len);
}
