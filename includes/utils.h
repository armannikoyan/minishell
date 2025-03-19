/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 18:14:57 by anikoyan          #+#    #+#             */
/*   Updated: 2025/03/19 20:10:06 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <unistd.h>
# include <errno.h>
# include <termios.h>
# include <term.h>
# include <curses.h>
# include <signal.h>
# include <stdbool.h>

// --------- TERMINAL SETTINGS ---------
void	set_term_config(struct termios *original);
void	restore_terminal_settings(struct termios *original);

// --------- SIGNAL HANDLERS ---------
void	psig_set(void);
void	psig_handler(int signo);

// --------- HELPER FUNCTIONS ---------
void	move_cursor_up(void);

// --------- ERROR PRINTING ---------
void	print_error(const char *message, int errorno);

#endif
