/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 17:26:37 by lvarnach          #+#    #+#             */
/*   Updated: 2026/02/02 21:06:51 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static bool	is_all_space(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
			return (false);
		++i;
	}
	return (true);
}

static char	*get_input(int *eof_count, t_hash_table *ht)
{
	char	*input;
	char	*line;
	t_entry	*entry;

	if (isatty(STDIN_FILENO))
		input = readline("minishell$ ");
	else
	{
		line = get_next_line(STDIN_FILENO);
		input = ft_strtrim(line, "\n");
		free(line);
	}
	if (!input)
	{
		(*eof_count)--;
		if (*eof_count < 0)
			return (ft_strdup("exit"));
		printf("Use \"exit\" to leave the shell.\n");
	}
	entry = ht_get(ht, "IGNOREEOF");
	if (entry == NULL)
		*eof_count = 0;
	else if (entry->val)
		*eof_count = ft_atoi(entry->val);
	return (input);
}

t_hash_table	*ht_create(void)
{
	t_hash_table	*ht;

	ht = (t_hash_table *)malloc(sizeof(t_hash_table));
	if (!ht)
	{
		print_error("minishell: ht_create: malloc", false);
		return (NULL);
	}
	ht->size = INITIAL_SIZE;
	ht->count = 0;
	ht->buckets = ft_calloc(ht->size, sizeof(t_entry *));
	if (!ht->buckets)
	{
		print_error("minishell: ht_create: ft_calloc", false);
		return (NULL);
	}
	return (ht);
}

t_hash_table	*setup_ht(char **envp, int *eof_count)
{
	t_hash_table	*ht;
	t_entry			*entry;

	ht = ht_create();
	if (!ht)
		exit(EXIT_FAILURE);
	insert_env(ht, envp);
	entry = ht_get(ht, "IGNOREEOF");
	if (entry == NULL)
		*eof_count = 0;
	else if (entry->val)
		*eof_count = ft_atoi(entry->val);
	return (ht);
}

void	interactive_loop(t_hash_table *ht, int errnum, int eof_count)
{
	int							hc;
	t_ast_node					*root;
	char						*input;

	while (true)
	{
		input = get_input(&eof_count, ht);
		if (!input)
			continue ;
		if (!is_all_space(input))
			add_history(input);
		root = tokenize(input, &errnum);
		free(input);
		if (root != NULL && syntax_check(root, &errnum) != SYNTAX_ERROR)
		{
			hc = 0;
			if (!scan_and_process_heredocs(root, ht, root, &hc))
				errnum = execute(root, ht, errnum, &(t_garbage)
					{.stack = NULL, .root = root, .ht = ht, .next = NULL});
			cleanup_heredoc_files(hc);
		}
		ast_deletion(root);
	}
}
