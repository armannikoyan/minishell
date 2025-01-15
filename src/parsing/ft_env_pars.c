/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_pars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 23:19:20 by gsimonia          #+#    #+#             */
/*   Updated: 2025/01/15 23:24:18 by gsimonia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

unsigned short	calculate_env_length(t_env_context *ctx)
{
	ctx->i = 0;
	ctx->single_quote = false;
	ctx->double_quote = false;
	return (calculate_env_length_helper(ctx));
}

void	handle_quotes_in_output(t_env_context *ctx)
{
	if (ctx->input[ctx->i] == '\'' && !ctx->double_quote)
	{
		ctx->single_quote = !ctx->single_quote;
	}
	else if (ctx->input[ctx->i] == '"' && !ctx->single_quote)
	{
		ctx->double_quote = !ctx->double_quote;
	}
	ctx->output[ctx->j++] = ctx->input[ctx->i++];
}

void	handle_dollar_in_output(t_env_context *ctx)
{
	char	*env_value;

	ctx->i++;
	if (ctx->input[ctx->i] && ft_isalnum(ctx->input[ctx->i]))
	{
		env_value = ft_get_env(ctx->input + ctx->i, ctx->envp);
		ft_strlcpy(ctx->output + ctx->j, env_value, ft_strlen(env_value) + 1);
		ctx->j += ft_strlen(env_value);
		ctx->i += ft_get_env_name_len(ctx->input + ctx->i);
	}
	else
	{
		ctx->output[ctx->j++] = '$';
		if (ctx->input[ctx->i])
		{
			ctx->output[ctx->j++] = ctx->input[ctx->i++];
		}
	}
}
