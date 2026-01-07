#include "../../includes/hash_table.h"
#include "../../includes/utils.h"
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

void	ht_resize(t_hash_table *ht, int new_size)
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
		//TODO: make normal error
		print_error("minishell: Failed to allocate memory", true);
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
