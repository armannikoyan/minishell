/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 16:25:19 by lvarnach          #+#    #+#             */
/*   Updated: 2026/01/26 16:25:41 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_H
# define EXPANSION_H

# include <stddef.h>

# include "hash_table.h"

size_t	env_var_len(char *str);

char	*get_env_val(char *str, t_hash_table *ht);

char	*expand_dollar_sign(char *str, t_hash_table *ht, int errnum);

#endif
