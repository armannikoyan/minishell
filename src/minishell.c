/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 20:17:13 by anikoyan          #+#    #+#             */
/*   Updated: 2025/03/17 19:46:27 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

extern void	rl_clear_history(void);

void	interactive_loop(void)
{
	char	*input;

	while (true)
	{
		input = readline("minishell$ ");
		if (!input)
		{
			move_cursor_up();
			ft_printf("minishell$ exit\n");
			break ;
		}
		add_history(input);
		free(input);
	}
	rl_clear_history();
}
