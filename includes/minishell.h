/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:57:19 by anikoyan          #+#    #+#             */
/*   Updated: 2025/03/12 20:18:06 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <errno.h>
# include <termios.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>

# include "../libs/libft/libft.h"

// --------- MINISHELL ENTRY ---------
void	interactive_loop(char **shell_env);

// --------- TERMINAL SETTINGS ---------
void	set_term_config(struct termios *original);
void	restore_terminal_settings(struct termios *original);

// --------- SIGNAL HANDLERS ---------
void	psig_set(void);
void	psig_handler(int signo);

// --------- HELPER FUNCTIONS ---------
char	**dup_shell_env(char **envp);
void	move_cursor_up(void);

// --------- ERROR PRINTING ---------
void	print_error(char *err);

#endif
