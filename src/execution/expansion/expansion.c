/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_sign_expansion.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 21:40:46 by lvarnach          #+#    #+#             */
/*   Updated: 2026/01/27 21:59:37 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "expansion.h"
#include "hash_table.h"
#include "tokenization.h"
#include "utils.h"
#include "../../../libs/libft/libft.h"

static int	is_tilde(t_exp *ctx)
{
	if (ctx->src[ctx->i] == '~' && ctx->quote == 0)
	{
		if ((ctx->i == 0 || ctx->src[ctx->i - 1] == ' ')
			&& (ctx->src[ctx->i + 1] == '\0' || ctx->src[ctx->i + 1] == '/'))
			return (1);
	}
	return (0);
}

static void	proc_tilde(t_exp *ctx, int mode)
{
	char	*home;
	size_t	len;

	home = getenv("HOME");
	if (!home)
	{
		if (mode)
			ctx->dst[ctx->j++] = ctx->src[ctx->i++];
		else
		{
			ctx->len++;
			ctx->i++;
		}
		return ;
	}
	len = ft_strlen(home);
	if (mode)
		ft_memcpy(ctx->dst + ctx->j, home, len);
	if (mode)
		ctx->j += len;
	else
		ctx->len += len;
	ctx->i++;
}

static void	expand_loop(t_exp *ctx, int mode)
{
	ctx->i = 0;
	ctx->quote = 0;
	while (ctx->src[ctx->i])
	{
		set_quote_char(ctx->src[ctx->i], &ctx->quote);
		if (ctx->src[ctx->i] == '$' && ctx->quote != '\'')
			proc_dollar(ctx, mode);
		else if (is_tilde(ctx))
			proc_tilde(ctx, mode);
		else
		{
			if (mode)
				ctx->dst[ctx->j++] = ctx->src[ctx->i++];
			else
			{
				ctx->len++;
				ctx->i++;
			}
		}
	}
}

char	*expand_dollar_sign(char *str, t_hash_table *ht, int errnum)
{
	t_exp	ctx;

	ctx.src = str;
	ctx.dst = NULL;
	ctx.len = 0;
	ctx.j = 0;
	ctx.err = errnum;
	ctx.ht = ht;
	expand_loop(&ctx, 0);
	if (ctx.len == 0)
		return (ft_strdup(""));
	ctx.dst = (char *)ft_calloc(sizeof(char), ctx.len + 1);
	if (!ctx.dst)
	{
		print_error("minishell: malloc", false);
		free(str);
		return (NULL);
	}
	expand_loop(&ctx, 1);
	ctx.dst[ctx.j] = '\0';
	return (ctx.dst);
}
