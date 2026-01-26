/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term_settings.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 17:23:48 by lvarnach          #+#    #+#             */
/*   Updated: 2026/01/26 17:24:01 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TERM_SETTINGS_H
# define TERM_SETTINGS_H

# include <termios.h>

void	set_term_config(struct termios *original);

void	restore_terminal_settings(struct termios *original);

void	psig_set(void);

void	psig_handler(int signo);

#endif
