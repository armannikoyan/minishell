/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 20:17:13 by anikoyan          #+#    #+#             */
/*   Updated: 2025/04/12 13:41:41 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/hash_table.h"

extern void	rl_clear_history(void);

static void	ht_replace_incorrect_env(t_hash_table *ht)
{
	char	*shlvl;
	char	*str;

	shlvl = ht_get(ht, "SHLVL");
	if (shlvl)
		str = ft_itoa(ft_atoi(ht_get(ht, "SHLVL")) + 1);
	else
		str = ft_itoa(1);
	if (!str)
	{
		print_error("minishell: Failed to duplicate environment.\n");
		exit(EXIT_FAILURE);
	}
	ht_insert(ht, "SHLVL", str);
	free(str);
	str = (char *)malloc(sizeof(char) * PATH_MAX);
	if (!str)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	ht_insert(ht, "PWD", getcwd(str, PATH_MAX));
	free(str);
}

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
	ht_replace_incorrect_env(ht);
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
	ht_insert_env(ht, envp);
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
	ht_destroy(ht);
	rl_clear_history();
}
