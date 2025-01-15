/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_expansion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 14:49:16 by anikoyan          #+#    #+#             */
/*   Updated: 2025/01/15 23:23:58 by gsimonia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	process_input_helper(t_env_context *ctx)
{
	while (ctx->input[ctx->i])
	{
		if (ctx->input[ctx->i] == '\'' || ctx->input[ctx->i] == '"')
		{
			handle_quotes_in_output(ctx);
		}
		else if (ctx->input[ctx->i] == '$' && !ctx->single_quote)
		{
			handle_dollar_in_output(ctx);
		}
		else
		{
			ctx->output[ctx->j++] = ctx->input[ctx->i++];
		}
	}
}

void	process_input(t_env_context *ctx)
{
	ctx->j = 0;
	ctx->single_quote = false;
	ctx->double_quote = false;
	process_input_helper(ctx);
	ctx->output[ctx->j] = '\0';
}

char	*ft_finalize_output(char **output)
{
	char	*tmp;

	tmp = *output;
	*output = ft_space_correction(*output);
	free(tmp);
	return (*output);
}

char	*ft_env_expansion(char *input, char **envp)
{
	t_env_context	ctx;
	unsigned short	len;

	if (!input || !envp)
		return (NULL);
	ctx.input = input;
	ctx.envp = envp;
	len = calculate_env_length(&ctx);
	ctx.output = (char *)malloc(sizeof(char) * (len + 1));
	if (!ctx.output)
		return (NULL);
	ctx.i = 0;
	process_input(&ctx);
	return (ft_finalize_output(&ctx.output));
}

void	free_env_context(t_env_context *ctx)
{
	if (ctx->output)
		free(ctx->output);
}
