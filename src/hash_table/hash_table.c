#include <stdio.h>
#include <stdlib.h>

#include "hash_table.h"
#include "utils.h"
#include "../../libs/libft/libft.h"

t_hash_table	*ht_create(void)
{
	t_hash_table	*ht;

	ht = (t_hash_table *)malloc(sizeof(t_hash_table));
	if (!ht)
	{
		print_error("minishell: malloc", false);
		exit(EXIT_FAILURE);
	}
	ht->size = INITIAL_SIZE;
	ht->count = 0;
	ht->buckets = ft_calloc(ht->size, sizeof(t_entry *));
	if (!ht->buckets)
	{
		print_error("minishell: calloc", false);
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
		print_error("minishell: malloc", false);
		exit(EXIT_FAILURE);
	}
	new_entry->key = ft_strdup(key);
	new_entry->value = ft_strdup(value);
	new_entry->next = ht->buckets[index];
	ht->buckets[index] = new_entry;
	ht->count++;
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
