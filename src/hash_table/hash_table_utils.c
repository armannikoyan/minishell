/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_table_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 19:35:42 by lvarnach          #+#    #+#             */
/*   Updated: 2026/01/26 19:44:49 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hash_table.h"
#include "utils.h"
#include "../../libs/libft/libft.h"

int	next_prime(int current)
{
	int	candidate;

	candidate = current * 2;
	while (true)
	{
		if (ft_isprime(candidate))
			return (candidate);
		++candidate;
	}
}

int	prev_prime(int current)
{
	int	desired;

	desired = current / 2;
	if (desired < MIN_SIZE)
		return (MIN_SIZE);
	while (desired >= MIN_SIZE)
	{
		if (ft_isprime(desired))
			return (desired);
		--desired;
	}
	return (MIN_SIZE);
}

unsigned long	hash_func(const char *str)
{
	unsigned long	hash;

	hash = 5381;
	while (*str)
		hash = ((hash << 5) + hash) + *str++;
	return (hash);
}

static void	copy_buckets(t_hash_table *ht, t_entry **new_buckets, int new_size)
{
	t_entry	*entry;
	t_entry	*next;
	int		index;
	int		i;

	i = -1;
	while (++i < ht->size)
	{
		entry = ht->buckets[i];
		while (entry)
		{
			next = entry->next;
			index = hash_func(entry->key) % new_size;
			entry->next = new_buckets[index];
			new_buckets[index] = entry;
			entry = next;
		}
	}
}

void	ht_resize(t_hash_table *ht, int new_size)
{
	t_entry	**new_buckets;

	new_buckets = (t_entry **) ft_calloc(new_size, sizeof(t_entry *));
	if (!new_buckets)
	{
		print_error("minishell: calloc", false);
		exit(EXIT_FAILURE);
	}
	copy_buckets(ht, new_buckets, new_size);
	free(ht->buckets);
	ht->buckets = new_buckets;
	ht->size = new_size;
}
