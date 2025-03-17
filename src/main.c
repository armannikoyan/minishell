/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:50:38 by anikoyan          #+#    #+#             */
/*   Updated: 2025/03/17 20:02:16 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/hash_table.h"

extern void	rl_clear_history(void);

int	main(int argc __attribute__((unused)),
		char **argv __attribute__((unused)), char **envp)
{
	struct termios		original_termios;
	t_hash_table		*ht;

	set_term_config(&original_termios);
	psig_set();
	ht = ht_create();
	if (!ht)
	{
		print_error("minishell: Failed to duplicate environment.\n");
		return (EXIT_FAILURE);
	}
	char *str;
	int i;
	int j;
	i = 0;
	j = 0;
	while (envp[i])
	{
		j = ft_strchr(envp[i], '=') - envp[i];
		str = malloc(sizeof(char) * (j + 1));
		ft_strlcpy(str, envp[i], j + 1);
		ht_insert(ht, str, &envp[i][j + 1]);
		free(str);
		i++;
	}
	interactive_loop();
	ft_printf("%s\n", ht_get(ht, "USER"));
	ft_printf("%s\n", ht_get(ht, "PATH"));
	ht_destroy(ht);
	restore_terminal_settings(&original_termios);
	return (EXIT_SUCCESS);
}
