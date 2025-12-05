/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:50:38 by anikoyan          #+#    #+#             */
/*   Updated: 2025/12/06 01:36:46 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <termios.h>
#include <stdlib.h>

#include "../includes/utils.h"
#include "../includes/minishell.h"

extern void	rl_clear_history(void);

int	main(int argc __attribute__((unused)),
		char **argv __attribute__((unused)), char **envp)
{
	struct termios		original_termios;

	set_term_config(&original_termios);
	psig_set();
	interactive_loop(envp);
	restore_terminal_settings(&original_termios);
	return (EXIT_SUCCESS);
}
