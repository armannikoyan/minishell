/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_expansion_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 17:14:08 by anikoyan          #+#    #+#             */
/*   Updated: 2025/01/15 23:26:04 by gsimonia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

unsigned short	ft_get_env_name_len(char *str)
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

unsigned short	handle_quotes(t_env_context *ctx)
{
	if (ctx->input[ctx->i] == '\'' && !ctx->double_quote)
	{
		ctx->single_quote = !ctx->single_quote;
	}
	else if (ctx->input[ctx->i] == '"' && !ctx->single_quote)
	{
		ctx->double_quote = !ctx->double_quote;
	}
	return (1);
}

unsigned short	handle_dollar(t_env_context *ctx)
{
	char			*env_value;
	unsigned short	length;

	length = 0;
	ctx->i++;
	if (ctx->input[ctx->i] && ft_isalnum(ctx->input[ctx->i]))
	{
		env_value = ft_get_env(ctx->input + ctx->i, ctx->envp);
		length += ft_strlen(env_value);
		ctx->i += ft_get_env_name_len(ctx->input + ctx->i) - 1;
	}
	else
	{
		length += 1;
		if (ctx->input[ctx->i])
		{
			length += 1;
			ctx->i++;
		}
		ctx->i--;
	}
	return (length);
}

unsigned short	calculate_env_length_helper(t_env_context *ctx)
{
	unsigned short	length;

	length = 0;
	while (ctx->input[ctx->i])
	{
		if (ctx->input[ctx->i] == '\'' || ctx->input[ctx->i] == '"')
		{
			length += handle_quotes(ctx);
		}
		else if (ctx->input[ctx->i] == '$' && !ctx->single_quote)
		{
			length += handle_dollar(ctx);
		}
		else
		{
			length++;
		}
		ctx->i++;
	}
	return (length);
}
