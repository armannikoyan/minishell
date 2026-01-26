/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term_config.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 19:06:38 by lvarnach          #+#    #+#             */
/*   Updated: 2026/01/26 19:06:39 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <termios.h>
#include <unistd.h>

#include "utils.h"

void	restore_terminal_settings(struct termios *original)
{
	tcsetattr(STDIN_FILENO, TCSANOW, original);
}

void	set_term_config(struct termios *original)
{
	struct termios	new_termios;

	if (tcgetattr(STDIN_FILENO, original) == -1)
		print_error("minishell: tcgetattr", false);
	new_termios = *original;
	new_termios.c_lflag |= ICANON;
	new_termios.c_lflag &= ~(ECHOCTL);
	if (tcsetattr(STDIN_FILENO, TCSANOW, &new_termios) == -1)
		print_error("minishell: tcsetattr", false);
}
