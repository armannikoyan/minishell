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
		//TODO: make normal error
		print_error("minishell: malloc", false);
		return (NULL);
	}
	ht->size = INITIAL_SIZE;
	ht->count = 0;
	ht->buckets = ft_calloc(ht->size, sizeof(t_entry *));
	if (!ht->buckets)
	{
		//TODO: make normal error
		print_error("minishell: ft_calloc", false);
		return (NULL);
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
			free(entry->val);
			free(entry);
			entry = next;
		}
		++i;
	}
	free(ht->buckets);
	free(ht);
}

t_entry	*ht_get(t_hash_table *ht, const char *key)
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
			return (entry);
		entry = entry->next;
	}
	return (NULL);
}

int	ht_create_bucket(t_hash_table *ht, const char *key, const char *value, bool is_local) {
	t_entry	*new_entry;
	unsigned long	hash;
	int				index;

	if ((float)ht->count / ht->size >= MAX_LOAD_FACTOR)
		ht_resize(ht, next_prime(ht->size));
	hash = hash_func(key);
	index = hash % ht->size;
	new_entry = (t_entry *)malloc(sizeof(t_entry));
	if (!new_entry)
	{
		//TODO: make normal error
		print_error("minishell: malloc", false);
		return (-1);
	}
	new_entry->key = ft_strdup(key);
	if (value != NULL)
		new_entry->val = ft_strdup(value);
	else
		new_entry->val = NULL;
	new_entry->is_local = is_local;
	new_entry->next = ht->buckets[index];
	ht->buckets[index] = new_entry;
	ht->count++;
	return (0);
}

// 0 - success
// 1 - entry doesn't exist
// 2 - strdup failed
int	ht_update_value(t_hash_table *ht, const char *key, const char *value)
{
	t_entry			*entry;
	char			*new_value;

	entry = ht_get(ht, key);
	if (!entry)
		return (1);
	if (value != NULL) {
		new_value = ft_strdup(value);
		if (new_value == NULL) {
			// TODO: make normal error
			print_error("minishell: ht_update_value: ft_strdup", false);
			// print_error("minishell: malloc", false);
			return (2);
		}
	}
	else
		new_value = NULL;
	free(entry->val);
	entry->val = new_value;
	return (0);
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
			free(entry->val);
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
