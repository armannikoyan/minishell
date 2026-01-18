#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "minishell.h"

#include <sys/errno.h>

#include "hash_table.h"
#include "utils.h"
#include "../libs/libft/libft.h"
#include "tokenization.h"
#include "execution.h"

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
		print_error("minishell: Failed to duplicate environment.\n", true);
		exit(EXIT_FAILURE);
	}
	ht_insert(ht, "SHLVL", str);
	free(str);
	str = (char *)malloc(sizeof(char) * PATH_MAX);
	if (!str)
	{
		print_error("minishell: malloc", false);
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
			print_error("minishell: malloc", false);
			exit(EXIT_FAILURE);
		}
		ft_strlcpy(str, envp[i], j + 1);
		ht_insert(ht, str, &envp[i][j + 1]);
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
	t_ast_node	*node;
	int	errno_buff;

	ht = ht_create();
	if (!ht)
	{
		print_error("minishell: Failed to duplicate environment.\n", true);
		exit(EXIT_FAILURE);
	}
	ht_insert_env(ht, envp);
	while (true)
	{
		errno_buff = errno;
		input = readline("minishell$ ");
		if (!input)
		{
			input = ft_strdup("exit");
			if (!input)
				print_error("minishell: malloc", false);
		}
		errno = errno_buff;
		if (isallspace(input)) {
			free(input);
			continue ;
		}
		add_history(input);
		node = tokenize(input);
		if (node == NULL) {
			free(input);
			continue ;
		}
		execute(node, ht);
		free(input);
	}
	ht_destroy(ht);
	rl_clear_history();
}