/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_expansion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 14:49:16 by anikoyan          #+#    #+#             */
/*   Updated: 2024/09/26 18:03:34 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	ft_is_valid_env(char *env)
{
	unsigned int	i;

	ft_printf("valid env arg: %s\n", env);
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

static unsigned int	ft_env_strlen(char *input, unsigned int i)
{
	unsigned int	len;

	len = 0;
	while (input[i] && (!ft_isspace(input[i]) || input[i] != '\''
			|| input[i] != '"' || !ft_isoperator(&input[i])))
		len++;
	return (len);
}

static char	**ft_find_env(char *input, char **env_output)
{
	unsigned int	i;
	unsigned int	j;
	unsigned int	quote_count;
	unsigned short	env_len;

	env_len = 0;
	i = 0;
	quote_count = 0;
	while (input[i])
	{
		while (input[i] && input[i] != '$')
		{
			if (input[i] == '\'')
				quote_count++;
			i++;
		}
		if (input[i] && input[i] == '$' && quote_count % 2 == 0
			&& ft_is_valid_env(&(input[++i])))
			env_len++;
		i++;
	}
	env_output = (char **)malloc(sizeof(char *) * (env_len + 1));
	if (!env_output)
		exit(EXIT_FAILURE);
	i = 0;
	quote_count = 0;
	while (input[i])
	{
		while (input[i] && input[i] != '$')
		{
			if (input[i] == '\'')
				quote_count++;
			i++;
		}
		if (input[i] && input[i] == '$' && quote_count % 2 == 0
			&& ft_is_valid_env(&(input[++i])))
		{
			// insertion into env_output
		}
		i++;
	}
	return (env_output);
}

char	*ft_env_expansion(char *input, char **envp)
{
	char	**env_output;

	if (!input)
		return (NULL);
	env_output = NULL;
	env_output = ft_find_env(input, env_output);
	return (NULL);
}
