/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_table_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:54:25 by anikoyan          #+#    #+#             */
/*   Updated: 2025/03/19 16:25:24 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/hash_table.h"
#include "../../includes/utils.h"

static int	next_prime(int current)
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

static int	prev_prime(int current)
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

static unsigned long	hash_func(const char *str)
{
	unsigned long	hash;

	hash = 5381;
	while (*str)
		hash = ((hash << 5) + hash) + *str++;
	return (hash);
}

static void	ht_resize(t_hash_table *ht, int new_size)
{
	t_entry			**new_buckets;
	t_entry			*entry;
	t_entry			*next;
	unsigned long	hash;
	int				index;
	int				i;

	new_buckets = (t_entry **)ft_calloc(new_size, sizeof(t_entry *));
	if (!new_buckets)
	{
		print_error("minishell: Failed to allocate memory");
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (i < ht->size)
	{
		entry = ht->buckets[i];
		while (entry)
		{
			next = entry->next;
			hash = hash_func(entry->key);
			index = hash % new_size;
			entry->next = new_buckets[index];
			new_buckets[index] = entry;
			entry = next;
		}
		++i;
	}
	free(ht->buckets);
	ht->buckets = new_buckets;
	ht->size = new_size;
}

void	ht_insert(t_hash_table *ht, const char *key, const char *value)
{
	t_entry			*entry;
	t_entry			*new_entry;
	unsigned long	hash;
	int				index;

	if ((float)ht->count / ht->size >= MAX_LOAD_FACTOR)
		ht_resize(ht, next_prime(ht->size));
	hash = hash_func(key);
	index = hash % ht->size;
	entry = ht->buckets[index];
	while (entry)
	{
		if (ft_strcmp(entry->key, key) == 0)
		{
			free(entry->value);
			entry->value = ft_strdup(value);
			return ;
		}
		entry = entry->next;
	}
	new_entry = (t_entry *)malloc(sizeof(t_entry));
	if (!new_entry)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	new_entry->key = ft_strdup(key);
	new_entry->value = ft_strdup(value);
	new_entry->next = ht->buckets[index];
	ht->buckets[index] = new_entry;
	ht->count++;
}

char	*ht_get(t_hash_table *ht, const char *key)
{
	t_entry			*entry;
	unsigned long	hash;
	int				index;

	hash = hash_func(key);
	index = hash % ht->size;
	entry = ht->buckets[index];
	while (entry)
	{
		if (ft_strcmp(entry->key, key) == 0)
			return (entry->value);
		entry = entry->next;
	}
	return (NULL);
}

void	ht_delete(t_hash_table *ht, const char *key)
{
	t_entry			*entry;
	t_entry			*prev;
	unsigned long	hash;
	int				index;

	hash = hash_func(key);
	index = hash % ht->size;
	prev = NULL;
	entry = ht->buckets[index];
	while (entry)
	{
		if (ft_strcmp(entry->key, key) == 0)
		{
			if (prev)
				prev->next = entry->next;
			else
				ht->buckets[index] = entry->next;
			free(entry->key);
			free(entry->value);
			free(entry);
			ht->count--;
			if (ht->size > MIN_SIZE
				&& (float)ht->count / ht->size <= MIN_LOAD_FACTOR)
				ht_resize(ht, prev_prime(ht->size));
			return ;
		}
		prev = entry;
		entry = entry->next;
	}
}
