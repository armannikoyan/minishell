/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 01:29:34 by lvarnach          #+#    #+#             */
/*   Updated: 2026/01/28 01:30:42 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

#include "hash_table.h"
#include "tokenization.h"
#include "../../../libs/libft/libft.h"

size_t	env_var_len(char *str)
{
	size_t	i;

	if (!str)
		return (0);
	i = 0;
	if (ft_isalpha(str[i]) || str[i] == '_')
		++i;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		++i;
	return (i);
}

char	*get_env_val(char *str, t_hash_table *ht)
{
	char		*key;
	char		*val;
	size_t		len;

	len = env_var_len(str);
	key = ft_substr(str, 0, len);
	if (!key)
		return (NULL);
	if (ht_get(ht, key) == NULL)
		return (NULL);
	val = ht_get(ht, key)->val;
	free(key);
	return (val);
}

static void	expand_var(t_exp *ctx, int mode, size_t len)
{
	char	*val;
	char	*tmp;

	ctx->i++;
	tmp = NULL;
	if (ctx->src[ctx->i] == '?')
	{
		val = ft_itoa(ctx->err);
		tmp = val;
	}
	else
		val = get_env_val(&ctx->src[ctx->i], ctx->ht);
	if (val)
		len = ft_strlen(val);
	if (mode && val)
		ft_memcpy(ctx->dst + ctx->j, val, len);
	if (mode)
		ctx->j += len;
	else
		ctx->len += len;
	free(tmp);
	if (ctx->src[ctx->i] == '?')
		ctx->i++;
	else
		ctx->i += env_var_len(&ctx->src[ctx->i]);
}

void	proc_dollar(t_exp *ctx, int mode)
{
	if (ctx->src[ctx->i + 1] == '?' || ft_isalpha(ctx->src[ctx->i + 1])
		|| ctx->src[ctx->i + 1] == '_')
		expand_var(ctx, mode, 0);
	else if (ctx->quote == 0 && (ctx->src[ctx->i + 1] == '\''
			|| ctx->src[ctx->i + 1] == '\"'))
		ctx->i++;
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
