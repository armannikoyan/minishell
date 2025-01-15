/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_expansion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 14:49:16 by anikoyan          #+#    #+#             */
/*   Updated: 2025/01/15 23:03:44 by gsimonia         ###   ########.fr       */
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

static unsigned short	calculate_env_length(t_env_context *ctx)
{
	unsigned short	len;
	char			*env_value;

	len = 0;
	ctx->i = 0;
	ctx->single_quote = false;
	ctx->double_quote = false;
	while (ctx->input[ctx->i])
	{
		if (ctx->input[ctx->i] == '\'' && !ctx->double_quote)
			ctx->single_quote = !ctx->single_quote;
		else if (ctx->input[ctx->i] == '"' && !ctx->single_quote)
			ctx->double_quote = !ctx->double_quote;
		else if (ctx->input[ctx->i] == '$' && !ctx->single_quote)
		{
			ctx->i++;
			if (ctx->input[ctx->i] && ft_isalnum(ctx->input[ctx->i]))
			{
				env_value = ft_get_env(ctx->input + ctx->i, ctx->envp);
				len += ft_strlen(env_value);
				ctx->i += ft_get_env_name_len(ctx->input + ctx->i) - 1;
			}
			else
			{
				len++;
				if (ctx->input[ctx->i])
					len++;
				ctx->i--;
			}
		}
		len++;
		ctx->i++;
	}
	return (len);
}

void process_input(t_env_context *ctx)
{
	ctx->j = 0;
	ctx->single_quote = false;
	ctx->double_quote = false;
	while (ctx->input[ctx->i])
	{
		if (ctx->input[ctx->i] == '\'' && !ctx->double_quote)
		{
			ctx->single_quote = !ctx->single_quote;
			ctx->output[ctx->j++] = ctx->input[ctx->i++];
		}
		else if (ctx->input[ctx->i] == '"' && !ctx->single_quote)
		{
			ctx->double_quote = !ctx->double_quote;
			ctx->output[ctx->j++] = ctx->input[ctx->i++];
		}
		else if (ctx->input[ctx->i] == '$' && !ctx->single_quote)
		{
			ctx->i++;
			if (ctx->input[ctx->i] && ft_isalnum(ctx->input[ctx->i]))
			{
				char *env_value = ft_get_env(ctx->input + ctx->i, ctx->envp);
				ft_strlcpy(ctx->output + ctx->j, env_value, ft_strlen(env_value) + 1);
				ctx->j += ft_strlen(env_value);
				ctx->i += ft_get_env_name_len(ctx->input + ctx->i);
			}
			else
			{
				ctx->output[ctx->j++] = '$';
				if (ctx->input[ctx->i])
					ctx->output[ctx->j++] = ctx->input[ctx->i++];
			}
		}
		else
			ctx->output[ctx->j++] = ctx->input[ctx->i++];
	}
	ctx->output[ctx->j] = '\0';
}

char *ft_finalize_output(char **output)
{
	char *tmp = *output;
	*output = ft_space_correction(*output);
	free(tmp);
	return (*output);
}

char *ft_env_expansion(char *input, char **envp)
{
	t_env_context ctx;

	if (!input || !envp)
		return NULL;
	ctx.input = input;
	ctx.envp = envp;
	unsigned short len = calculate_env_length(&ctx);
	ctx.output = (char *)malloc(sizeof(char) * (len + 1));
	if (!ctx.output)
		return NULL;

	ctx.i = 0;
	process_input(&ctx);
	return ft_finalize_output(&ctx.output);
}
