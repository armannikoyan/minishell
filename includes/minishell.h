/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:57:19 by anikoyan          #+#    #+#             */
/*   Updated: 2025/03/19 12:11:21 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>

# include "../libs/libft/libft.h"
# include "utils.h"
# include "hash_table.h"

// --------- MINISHELL ENTRY ---------
void	interactive_loop(char **envp);

#endif
