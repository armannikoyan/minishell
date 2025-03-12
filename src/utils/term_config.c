/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term_config.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 13:51:51 by anikoyan          #+#    #+#             */
/*   Updated: 2025/03/12 20:16:29 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	restore_terminal_settings(struct termios *original)
{
	tcsetattr(STDIN_FILENO, TCSANOW, original);
}

void	set_term_config(struct termios *original)
{
	struct termios	new_termios;

	tcgetattr(STDIN_FILENO, original);
	new_termios = *original;
	new_termios.c_lflag &= ~ECHOCTL;
	new_termios.c_cc[VMIN] = 1;
	new_termios.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
}
