/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_table.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:15:58 by anikoyan          #+#    #+#             */
/*   Updated: 2025/04/12 14:49:16 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HASH_TABLE_H
# define HASH_TABLE_H

# include <unistd.h>
# include <stdio.h>
# include <stdbool.h>

# include "../libs/libft/libft.h"

# define MAX_LOAD_FACTOR 0.7
# define MIN_LOAD_FACTOR 0.7
# define INITIAL_SIZE 7
# define MIN_SIZE 7

typedef struct s_entry
{
	char			*key;
	char			*value;
	struct s_entry	*next;
}	t_entry;

typedef struct s_hash_table
{
	t_entry	**buckets;
	int		size;
	int		count;
}	t_hash_table;

t_hash_table	*ht_create(void);
void			ht_destroy(t_hash_table *ht);
void			ht_insert(t_hash_table *ht, const char *key, const char *value);
char			*ht_get(t_hash_table *ht, const char *key);
void			ht_delete(t_hash_table *ht, const char *key);

int				next_prime(int current);
int				prev_prime(int current);
unsigned long	hash_func(const char *str);
void			ht_resize(t_hash_table *ht, int new_size);

#endif
