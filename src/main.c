/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:50:38 by anikoyan          #+#    #+#             */
/*   Updated: 2025/03/12 20:18:37 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

extern void	rl_clear_history(void);
extern void	rl_replace_line(const char *text, int clear_undo);

int	main(int argc __attribute__((unused)),
		char **argv __attribute__((unused)), char **envp)
{
	struct termios		original_termios;
	char				**shell_env;

	set_term_config(&original_termios);
	psig_set();
	shell_env = dup_shell_env(envp);
	if (!shell_env)
	{
		print_error("minishell: Failed to duplicate environment.\n");
		return (EXIT_FAILURE);
	}
	interactive_loop(shell_env);
	rl_clear_history();
	restore_terminal_settings(&original_termios);
	return (EXIT_SUCCESS);
}
