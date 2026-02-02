/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 16:38:23 by lvarnach          #+#    #+#             */
/*   Updated: 2026/02/02 21:05:12 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "hash_table.h"

t_hash_table	*setup_ht(char **envp, int *eof_count);
void			interactive_loop(t_hash_table *ht, int errnum, int eof_count);

#endif
