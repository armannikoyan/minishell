/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_expansion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 14:49:16 by anikoyan          #+#    #+#             */
/*   Updated: 2024/10/01 20:09:15 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	ft_find_env(char *input, char **env_output,
		unsigned short env_len, char **envp)
{
	unsigned short	i;
	unsigned short	j;
	unsigned short	k;

	i = USHRT_MAX;
	env_len = 0;
	while (input[++i])
	{
		if (input[i] && ft_get_quote_count(input, i) % 2 == 0
			&& input[i] == '$' && ft_is_valid_env(&(input[++i])))
		{
			j = 0;
			k = ft_envlen(input, i);
			while (envp && envp[j] && ft_strncmp(envp[j], &input[i], k))
				j++;
			if (envp[j] && envp[j][k++] == '=')
				env_output[env_len++] = ft_strjoin("", &envp[j][k]);
			else
				env_output[env_len++] = ft_strdup("");
		}
		env_output[env_len] = NULL;
	}
}

static unsigned short	ft_exp_strlen(char *input, char **env_output)
{
	unsigned short	i;
	unsigned short	len;

	i = 0;
	len = 0;
	while (input[i])
	{
		if (input[i] && ft_get_quote_count(input, i) % 2 == 0
			&& input[i] == '$' && ft_is_valid_env(&(input[++i])))
			i += ft_envlen(input, i);
		else
			len++;
		i++;
	}
	i = 0;
	while (env_output[i])
		len += ft_strlen(env_output[i++]);
	return (len);
}

static char	*ft_expand_env_variable(char *expanded_input,
	char **env_output, char *input, unsigned short *i)
{
	char			*tmp;
	unsigned short	j;
	unsigned short	k;

	j = 0;
	k = 0;
	tmp = expanded_input;
	expanded_input = ft_strjoin(tmp, env_output[k]);
	j += ft_strlen(env_output[k]);
	k++;
	free(tmp);
	*i += ft_envlen(input, *i) - 1;
	return (expanded_input);
}

static char	*ft_str_envjoin(char *input, char **env_output)
{
	unsigned short	i;
	unsigned short	j;
	unsigned short	k;
	char			*expanded_input;

	expanded_input = malloc(sizeof(char) * ft_exp_strlen(input, env_output));
	i = 0;
	j = 0;
	k = 0;
	while (input[i])
	{
		if (input[i] && ft_get_quote_count(input, i) % 2 == 0
			&& input[i] == '$' && ft_is_valid_env(&(input[++i])))
			expanded_input = ft_expand_env_variable(expanded_input,
					env_output, input, &i);
		else
		{
			expanded_input[j] = input[i];
			j++;
		}
		i++;
	}
	return (expanded_input);
}

// static char	*ft_str_envjoin(char *input, char **env_output)
// {
// 	unsigned short	i;
// 	unsigned short	j;
// 	unsigned short	k;
// 	char			*expanded_input;
// 	char			*tmp;

// 	expanded_input = malloc(sizeof(char) * ft_exp_strlen(input, env_output));
// 	i = 0;
// 	j = 0;
// 	k = 0;
// 	while (input[i])
// 	{
// 		if (input[i] && ft_get_quote_count(input, i) % 2 == 0
// 			&& input[i] == '$' && ft_is_valid_env(&(input[++i])))
// 		{
// 			tmp = expanded_input;
// 			expanded_input = ft_strjoin(tmp, env_output[k]);
// 			j += ft_strlen(env_output[k]);
// 			k++;
// 			free(tmp);
// 			i += ft_envlen(input, i) - 1;
// 		}
// 		else
// 		{
// 			expanded_input[j] = input[i];
// 			j++;
// 		}
// 		i++;
// 	}
// 	return (expanded_input);
// }

char	*ft_env_expansion(char *input, char **envp)
{
	char			**env_output;
	char			*expanded_input;
	unsigned short	i;
	unsigned short	env_len;

	if (!input)
		return (NULL);
	env_output = NULL;
	env_len = ft_varlen(input);
	env_output = (char **)malloc(sizeof(char *) * (env_len + 1));
	if (!env_output)
		exit(EXIT_FAILURE);
	ft_find_env(input, env_output, env_len, envp);
	if (!env_output[0])
	{
		free(env_output);
		return (ft_strdup(input));
	}
	expanded_input = ft_str_envjoin(input, env_output);
	i = 0;
	while (env_output[i])
		free(env_output[i++]);
	free(env_output);
	return (expanded_input);
}
