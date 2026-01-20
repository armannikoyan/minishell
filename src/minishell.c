#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "minishell.h"
#include "hash_table.h"
#include "utils.h"
#include "../libs/libft/libft.h"
#include "tokenization.h"

static void ht_update_shlvl(t_hash_table *ht) {
	char	*shlvl;
	char	*str;

	if (ht_get(ht, "SHLVL") == NULL)
	{
		if (ht_create_bucket(ht, "SHLVL", NULL, false) == -1) {
			//TODO: make normal error
			exit(EXIT_FAILURE);
		}
	}
	shlvl = ht_get(ht, "SHLVL")->val;
	if (shlvl)
		str = ft_itoa(ft_atoi(shlvl) + 1);
	else
		str = ft_itoa(1);
	if (!str)
	{
		//TODO: make normal error
		print_error("minishell: Failed to duplicate environment.\n", true);
		exit(EXIT_FAILURE);
	}
	ht_update_value(ht, "SHLVL", str);
	free(str);
}

static void	ht_replace_incorrect_env(t_hash_table *ht)
{
	char	*str;

	ht_update_shlvl(ht);
	str = (char *)malloc(sizeof(char) * PATH_MAX);
	if (!str)
	{
		//TODO: make normal error
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	if (getcwd(str, PATH_MAX) == NULL) {
		print_error("minishell: getcwd", false);
	}
	ht_create_bucket(ht, "PWD", str, false);
	if (ht_get(ht, "OLDPWD") == NULL)
		ht_create_bucket(ht, "OLDPWD", NULL, false);
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
			//TODO: make normal error
			perror("malloc");
			exit(EXIT_FAILURE);
		}
		ft_strlcpy(str, envp[i], j + 1);
		ht_create_bucket(ht, str, &envp[i][j + 1], false);
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
		//TODO: make normal error
		print_error("minishell: Failed to duplicate environment.\n", true);
		exit(EXIT_FAILURE);
	}
	ht_insert_env(ht, envp);
	ft_env(1, envp, ht);
	while (true)
	{
		input = readline("minishell$ ");
		if (!input)
			input = ft_strdup("exit");
		add_history(input);
		tokenize(input);
		free(input);
	}
	ht_destroy(ht);
	rl_clear_history();
}