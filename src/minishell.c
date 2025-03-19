/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 20:17:13 by anikoyan          #+#    #+#             */
/*   Updated: 2025/03/19 12:16:16 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/hash_table.h"

extern void	rl_clear_history(void);

static void	ht_insert_env(t_hash_table *ht, char **envp)
{
	char	*str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (envp[i])
	{
		j = ft_strchr(envp[i], '=') - envp[i];
		str = (char *)malloc(sizeof(char) * (j + 1));
		if (!str)
		{
			perror("malloc");
			exit(EXIT_FAILURE);
		}
		ft_strlcpy(str, envp[i], j + 1);
		ht_insert(ht, str, &envp[i][j + 1]);
		free(str);
		i++;
	}
}

void	interactive_loop(char	**envp)
{
	t_hash_table	*ht;
	char			*input;

	ht = ht_create();
	if (!ht)
	{
		print_error("minishell: Failed to duplicate environment.\n");
		exit(EXIT_FAILURE);
	}
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
	ft_printf("%s\n", ht_get(ht, "USER")); // remove
	ft_printf("%s\n", ht_get(ht, "PATH"));
	ht_destroy(ht);
	rl_clear_history();
}
