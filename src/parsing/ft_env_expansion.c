/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_expansion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 14:49:16 by anikoyan          #+#    #+#             */
/*   Updated: 2025/01/14 18:12:09 by gsimonia         ###   ########.fr       */
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

static unsigned short	calculate_non_env_length(char *input,
		unsigned short *i, bool *opened_quote)
{
	unsigned short	len;

	len = 0;
	while (input[*i]
		&& (input[*i] != '$' || (input[*i] == '$' && *opened_quote)))
	{
		if (input[*i] == '\'')
			*opened_quote = !*opened_quote;
		len++;
		(*i)++;
	}
	if (input[*i]
		&& ((input[*i + 1] && !ft_isalnum(input[*i + 1])) || !input[*i + 1]))
	{
		(*i)++;
		len++;
	}
	return (len);
}

static unsigned short	calculate_env_variable_length(char *input,
		unsigned short *i, char **envp)
{
	unsigned short	len;

	len = 0;
	if (input[*i])
	{
		(*i)++;
		if (input[*i] && input[*i] != '?')
		{
			len += ft_strlen(ft_get_env(input + *i, envp));
			len -= ft_get_env_name_len(input + *i);
		}
		else if (input[*i])
			len++;
	}
	return (len);
}

static unsigned short	calculate_env_length(char *input,
		char **envp)
{
	unsigned short		len;
	unsigned short		i;
	bool				opened_quote;

	len = 0;
	i = 0;
	opened_quote = false;
	while (input[i])
	{
		len += calculate_non_env_length(input, &i, &opened_quote);
		if (input[i])
			len += calculate_env_variable_length(input, &i, envp);
	}
	return (len);
}

void	ft_process_quotes(char *input,
		unsigned short *i, char *output, unsigned short *j)
{
	bool	opened_quote;

	opened_quote = false;
	while (input[*i]
		&& (input[*i] != '$' || (input[*i] == '$' && opened_quote)))
	{
		if (input[*i] == '\'')
			opened_quote = !opened_quote;
		output[(*j)++] = input[(*i)++];
	}
	return ;
}

char	*ft_handle_dollar_sign(char *input,
		char *output, unsigned short *i, unsigned short *j)
{
	output[*j] = input[*i];
	(*i)++;
	(*j)++;
	return (output);
}

char	*ft_finalize_output(char *output)
{
	char	*tmp;

	tmp = output;
	output = ft_space_correction(output);
	free(tmp);
	return (output);
}

void	process_input(char *input, char **envp, char *output, unsigned short *i)
{
	unsigned short	j;
	char			*tmp;

	j = 0;
	while (input[*i])
	{
		ft_process_quotes(input, i, output, &j);
		if (input[*i] && ((input[*i + 1]
					&& !ft_isalnum(input[*i + 1])) || !input[*i + 1]))
			output = ft_handle_dollar_sign(input, output, i, &j);
		else if (input[*i])
		{
			(*i)++;
			if (input[*i] && input[*i] != '?')
			{
				tmp = ft_get_env(input + *i, envp);
				ft_strcpy(output + j, tmp);
				j += ft_strlen(tmp);
				*i += ft_get_env_name_len(input + *i);
			}
			else if (input[*i])
				output[j++] = input[(*i)++];
		}
	}
	output[j] = '\0';
}

char	*ft_env_expansion(char *input, char **envp)
{
	char			*output;
	unsigned short	i;

	i = calculate_env_length(input, envp);
	output = (char *)malloc(sizeof(char) * (i + 1));
	if (!output)
		return (NULL);
	i = 0;
	process_input(input, envp, output, &i);
	return (ft_finalize_output(output));
}
