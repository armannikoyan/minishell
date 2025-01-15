/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_expansion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 14:49:16 by anikoyan          #+#    #+#             */
/*   Updated: 2025/01/15 16:08:05 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static unsigned short ft_get_env_name_len(char *str)
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


static unsigned short calculate_env_length(char *input, char **envp)
{
	unsigned short	len;
	unsigned short	i;
	bool			single_quote;
	bool			double_quote;

	len = 0;
	i = 0;
	single_quote = false;
	double_quote = false;
	while (input[i])
	{
		if (input[i] == '\'' && !double_quote)
			single_quote = !single_quote;
		else if (input[i] == '"' && !single_quote)
			double_quote = !double_quote;
		else if (input[i] == '$' && !single_quote)
		{
			i++;
			if (input[i] && ft_isalnum(input[i]))
			{
				char *env_value = ft_get_env(input + i, envp);
				len += ft_strlen(env_value);
				i += ft_get_env_name_len(input + i) - 1;
			}
			else
			{
				len++;
				if (input[i])
					len++;
				i--;
			}
		}
		len++;
		i++;
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

char	*ft_finalize_output(char **output)
{
	char	*tmp;

	tmp = *output;
	*output = ft_space_correction(*output);
	free(tmp);
	return (*output);
}

void process_input(char *input, char **envp, char *output, unsigned short *i)
{
    unsigned short j = 0;
    bool single_quote = false;
    bool double_quote = false;
    
    while (input[*i])
    {
        if (input[*i] == '\'' && !double_quote)
        {
            single_quote = !single_quote;
            output[j++] = input[(*i)++];
        }
        else if (input[*i] == '"' && !single_quote)
        {
            double_quote = !double_quote;
            output[j++] = input[(*i)++];
        }
        else if (input[*i] == '$' && !single_quote)
        {
            (*i)++;
            if (input[*i] && ft_isalnum(input[*i]))
            {
                char *env_value = ft_get_env(input + *i, envp);
                ft_strlcpy(output + j, env_value, ft_strlen(env_value) + 1);
                j += ft_strlen(env_value);
                *i += ft_get_env_name_len(input + *i);
            }
            else
            {
                output[j++] = '$';
                if (input[*i])
                    output[j++] = input[(*i)++];
            }
        }
        else
            output[j++] = input[(*i)++];
    }
    output[j] = '\0';
}

char *ft_env_expansion(char *input, char **envp)
{
    unsigned short	i;
    unsigned short	len;

    if (!input || !envp)
        return NULL;
    len = calculate_env_length(input, envp);
    char *output = (char *)malloc(sizeof(char) * (len + 1));
    if (!output)
        return NULL;
    i = 0;
    process_input(input, envp, output, &i);
    ft_finalize_output(&output);
    return (output);
}
