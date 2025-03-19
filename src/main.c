/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:50:38 by anikoyan          #+#    #+#             */
/*   Updated: 2025/03/19 20:33:05 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/utils.h"

extern void	rl_clear_history(void);

int	main(int argc __attribute__((unused)),
		char **argv __attribute__((unused)), char **envp)
{
	struct termios		original_termios;

	set_term_config(&original_termios);
	psig_set();
	interactive_loop((const char **)envp);
	restore_terminal_settings(&original_termios);
	return (EXIT_SUCCESS);
}
