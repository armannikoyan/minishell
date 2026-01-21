#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "minishell.h"

#include <sys/errno.h>

#include "error_codes.h"
#include "hash_table.h"
#include "utils.h"
#include "../libs/libft/libft.h"
#include "tokenization.h"
#include "execution.h"

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
		print_error("minishell: malloc", false);
		exit(EXIT_FAILURE);
	}
	if (getcwd(str, PATH_MAX) == NULL) {
		print_error("minishell: getcwd", false);
	}
	if (ht_get(ht, "PWD") == NULL)
		ht_create_bucket(ht, "PWD", str, false);
	else
		ht_update_value(ht, "PWD", str);
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
			print_error("minishell: malloc", false);
			exit(EXIT_FAILURE);
		}
		ft_strlcpy(str, envp[i], j + 1);
		ht_create_bucket(ht, str, &envp[i][j + 1], false);
		free(str);
		i++;
	}
	ht_replace_incorrect_env(ht);
}

static bool	isallspace(const char *str) {
	size_t	i;

	i = 0;
	while (str[i]) {
		if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
			return (false);
		++i;
	}
	return (true);
}

void	interactive_loop(char	**envp)
{
	t_hash_table	*ht;
	char			*input;
	t_ast_node	*root;
	int	errno_buff;
	int	eof_count;

	ht = ht_create();
	if (!ht)
	{
		print_error("minishell: Failed to duplicate environment.\n", true);
		exit(EXIT_FAILURE);
	}
	ht_insert_env(ht, envp);
	if (ht_get(ht, "IGNOREEOF") == NULL)
		eof_count = 0;
	else
		eof_count = ft_atoi(ht_get(ht, "IGNOREEOF")->val);
	while (true)
	{
		errno_buff = errno;
		input = readline("minishell$ ");
		if (!input)
		{
			eof_count--;
			if (eof_count < 0) {
				input = ft_strdup("exit");
				if (!input)
					print_error("minishell: malloc", false);
			}
			else {
				printf("Use \"exit\" to leave the shell.\n");
				continue;
			}
		}
		errno = errno_buff;
		if (!isallspace(input))
		{
			add_history(input);
			root = tokenize(input);
			if (root != NULL) {
				if (syntax_check(root) != SYNTAX_ERROR)
					execute(root, ht);
			}
		}
		if (ht_get(ht, "IGNOREEOF") == NULL)
			eof_count = 0;
		else
			eof_count = ft_atoi(ht_get(ht, "IGNOREEOF")->val);
		free(input);
	}
	ht_destroy(ht);
	rl_clear_history();
}