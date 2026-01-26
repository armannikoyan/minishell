/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 18:30:13 by lvarnach          #+#    #+#             */
/*   Updated: 2026/01/26 18:30:14 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <stdbool.h>

# include "hash_table.h"

void	free_split(char **cd_path);
void	insert_env(t_hash_table *ht, char **envp);
void	print_error(const char *err, bool is_custom_err);

#endif
