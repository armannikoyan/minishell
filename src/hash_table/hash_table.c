/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_table.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:17:00 by anikoyan          #+#    #+#             */
/*   Updated: 2025/03/19 16:22:48 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/hash_table.h"
#include "../../includes/utils.h"

t_hash_table	*ht_create(void)
{
	t_hash_table	*ht;

	ht = (t_hash_table *)malloc(sizeof(t_hash_table));
	if (!ht)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	ht->size = INITIAL_SIZE;
	ht->count = 0;
	ht->buckets = ft_calloc(ht->size, sizeof(t_entry *));
	if (!ht->buckets)
	{
		print_error("minishell: Failed to allocate memory");
		exit(EXIT_FAILURE);
	}
	return (ht);
}

void	ht_destroy(t_hash_table *ht)
{
	int		i;
	t_entry	*entry;
	t_entry	*next;

	i = 0;
	while (i < ht->size)
	{
		entry = ht->buckets[i];
		while (entry)
		{
			next = entry->next;
			free(entry->key);
			free(entry->value);
			free(entry);
			entry = next;
		}
		++i;
	}
	free(ht->buckets);
	free(ht);
}
