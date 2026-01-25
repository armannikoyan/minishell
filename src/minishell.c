#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "minishell.h"

#include "builtin.h"
#include "error_codes.h"
#include "hash_table.h"
#include "utils.h"
#include "../libs/libft/libft.h"
#include "tokenization.h"
#include "execution.h"
#include "../get_next_line/get_next_line.h"

static void update_shlvl(t_hash_table *ht) {
	char	*shlvl;
	char	*str;

	if (ht_get(ht, "SHLVL") == NULL)
	{
		if (ht_create_bucket(ht, "SHLVL", NULL, false) == -1)
			exit(EXIT_FAILURE);
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

static void	replace_incorrect_env(t_hash_table *ht)
{
	char	str[PATH_MAX];

	update_shlvl(ht);
	if (getcwd(str, sizeof(str)) == NULL)
		print_error("minishell: replace_incorrect_env: getcwd", false);
	if (ht_get(ht, "PWD") == NULL)
		ht_create_bucket(ht, "PWD", str, false);
	else
		ht_update_value(ht, "PWD", str);
	if (ht_get(ht, "OLDPWD") == NULL)
		ht_create_bucket(ht, "OLDPWD", NULL, false);
}

static void	insert_env(t_hash_table *ht, char **envp)
{
	char	*str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (envp[i])
	{
		// Тут можно попробовать сделать выделение одного буфера под все ключи. Пример найдёшь в экспорте
		j = ft_strchr(envp[i], '=') - envp[i];
		str = (char *)malloc(sizeof(char) * (j + 1));
		if (!str)
		{
			print_error("minishell: insert_env: malloc", false);
			exit(EXIT_FAILURE);
		}
		ft_strlcpy(str, envp[i], j + 1);
		ht_create_bucket(ht, str, &envp[i][j + 1], false);
		free(str);
		i++;
	}
	replace_incorrect_env(ht);
}

static bool	is_all_space(const char *str) {
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
	int	errnum;
	int	eof_count;

	errnum = 0; //TODO: pass to the execution for parsing ?
	ht = ht_create();
	if (!ht)
		exit(EXIT_FAILURE);
	insert_env(ht, envp);
	if (ht_get(ht, "IGNOREEOF") == NULL)
		eof_count = 0;
	else
		eof_count = ft_atoi(ht_get(ht, "IGNOREEOF")->val);
	while (true)
	{
		if (isatty(STDIN_FILENO))
			input = readline("minishell$ ");
		else {
			char *line;
			line = get_next_line(STDIN_FILENO);
			input = ft_strtrim(line, "\n");
			free(line);
		}
		if (!input)
		{
			eof_count--;
			if (eof_count < 0)
				ft_exit(1, &(char*){"exit"}, ht, errnum);
			else {
				printf("Use \"exit\" to leave the shell.\n");
				continue;
			}
		}
		if (!is_all_space(input))
		{
			add_history(input);
			root = tokenize(input, &errnum);
			int heredoc_counter = 0;
			if (root != NULL && scan_and_process_heredocs(root, ht, &heredoc_counter) == 0 &&
				syntax_check(root, &errnum) != SYNTAX_ERROR) {
				errnum = execute(root, ht, errnum);
			}
			cleanup_heredoc_files(heredoc_counter);
			ast_deletion(root);
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