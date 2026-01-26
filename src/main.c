/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 17:25:36 by lvarnach          #+#    #+#             */
/*   Updated: 2026/01/26 17:26:07 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <termios.h>
#include <unistd.h>

#include "minishell.h"
#include "term_settings.h"

int	main(int argc __attribute__((unused)),
		char **argv __attribute__((unused)), char **envp)
{
	struct termios		original_termios;
	int					is_tty;

	is_tty = isatty(STDIN_FILENO);
	if (is_tty)
	{
		set_term_config(&original_termios);
		psig_set();
	}
	interactive_loop(envp, 0);
	if (is_tty)
		restore_terminal_settings(&original_termios);
	return (0);
}
