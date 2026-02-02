/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 16:25:19 by lvarnach          #+#    #+#             */
/*   Updated: 2026/02/02 20:19:03 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_H
# define EXPANSION_H

# include <dirent.h>
# include <stddef.h>

# include "ast.h"
# include "hash_table.h"
# include "../libs/libft/libft.h"

size_t	env_var_len(char *str);
char	*get_env_val(char *str, t_hash_table *ht);
char	**list_to_argv(t_list *lst);
t_list	*get_matches(char *raw_token, DIR *dir);
char	**expand_wildcards(char **old_argv);
int		run_expansion(t_ast_node *node, t_hash_table *ht, int errnum);
char	*expand_dollar_sign(char *str, t_hash_table *ht, int errnum);

#endif
