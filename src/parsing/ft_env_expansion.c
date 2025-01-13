/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_expansion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 14:49:16 by anikoyan          #+#    #+#             */
/*   Updated: 2025/01/13 07:03:34 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static unsigned short	ft_get_env_name_len(char *str)
{
	unsigned short	i;

	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (i);
}

char	*ft_get_env(char *str, char **envp)
{
	char			*env;
	unsigned short	env_len;
	unsigned short	j;

	env_len = ft_get_env_name_len(str);
	env = (char *)malloc(sizeof(char) * (env_len + 2));
	if (!env)
		exit(EXIT_FAILURE);
	ft_strlcpy(env, str, env_len + 1);
	env[env_len] = '=';
	env[env_len + 1] = '\0';
	j = 0;
	while (envp[j])
	{
		if (ft_strncmp(env, envp[j], env_len + 1) == 0)
		{
			free(env);
			return (envp[j] + env_len + 1);
		}
		j++;
	}
	free(env);
	return ("");
}

char	*ft_env_expansion(char *input, char **envp)
{
	char			*output;
	char			*tmp;
	bool			opened_quote;
	unsigned short	len;
	unsigned short	i;
	unsigned short	j;

	len = 0;
	i = 0;
	opened_quote = false;
	while (input[i])
	{
		while (input[i] && (input[i] != '$' || (input[i] == '$'
					&& opened_quote)))
		{
			if (input[i] == '\'')
				opened_quote = !opened_quote;
			len++;
			i++;
		}
		if (input[i] && ((input[i + 1] && !ft_isalnum(input[i + 1]))
				|| !input[i + 1]))
		{
			i++;
			len++;
		}
		else if (input[i])
		{
			i++;
			if (input[i] && input[i] != '?')
			{
				len += ft_strlen(ft_get_env(input + i, envp));
				len -= ft_get_env_name_len(input + i);
			}
			else if (input[i])
				len++;
		}
	}
	output = (char *)malloc(sizeof(char) * (len + 1));
	if (!output)
		return (NULL);
	opened_quote = false;
	i = 0;
	j = 0;
	while (input[i])
	{
		while (input[i] && (input[i] != '$' || (input[i] == '$'
					&& opened_quote)))
		{
			if (input[i] == '\'')
				opened_quote = !opened_quote;
			output[j++] = input[i++];
		}
		if (input[i] && ((input[i + 1] && !ft_isalnum(input[i + 1]))
				|| !input[i + 1]))
		{
			output[j] = input[i];
			i++;
			j++;
		}
		else if (input[i])
		{
			i++;
			if (input[i] && input[i] != '?')
			{
				tmp = ft_get_env(input + i, envp);
				ft_strcpy(output + j, tmp);
				j += ft_strlen(tmp);
				i += ft_get_env_name_len(input + i);
			}
			else if (input[i])
				output[j++] = input[i++];
		}
	}
	output[j] = '\0';
	return (output);
}
