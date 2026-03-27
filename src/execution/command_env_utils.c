/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_env_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 19:44:55 by lvarnach          #+#    #+#             */
/*   Updated: 2026/01/28 19:44:56 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

#include "hash_table.h"
#include "../libs/libft/libft.h"

void	update_underscore(t_hash_table *ht, char *value)
{
	if (ht_get(ht, "_") == NULL)
		ht_create_bucket(ht, "_", value, false);
	else
		ht_update_value(ht, "_", value);
}

static char	*create_env_str(char *key, char *value)
{
	char	*tmp;
	char	*res;

	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return (NULL);
	res = ft_strjoin(tmp, value);
	free(tmp);
	return (res);
}

static void	fill_envp(t_hash_table *ht, char **envp)
{
	size_t	i;
	size_t	j;
	t_entry	*entry;

	i = 0;
	j = 0;
	while (i < (size_t)ht->size)
	{
		entry = ht->buckets[i];
		while (entry)
		{
			envp[j] = create_env_str(entry->key, entry->val);
			if (envp[j])
				j++;
			entry = entry->next;
		}
		i++;
	}
	envp[j] = NULL;
}

char	**ht_to_envp(t_hash_table *ht)
{
	char	**envp;

	envp = malloc(sizeof(char *) * (ht->count + 1));
	if (!envp)
		return (NULL);
	fill_envp(ht, envp);
	return (envp);
}
