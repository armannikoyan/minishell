/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 20:17:13 by anikoyan          #+#    #+#             */
/*   Updated: 2025/03/19 20:33:10 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/hash_table.h"
#include "../includes/utils.h"

extern void	rl_clear_history(void);

static char	*extract_key(const char *env_entry)
{
	char	*key;
	char	*delimiter_pos;
	int		key_length;

	if (!env_entry)
		return (NULL);
	delimiter_pos = ft_strchr(env_entry, '=');
	key_length = delimiter_pos - env_entry;
	key = (char *)malloc(sizeof(char) * (key_length + 1));
	if (!key)
		print_error("malloc", EXIT_FAILURE);
	ft_strlcpy(key, env_entry, key_length + 1);
	return (key);
}

static void	update_shlvl(t_hash_table *ht)
{
	char	*shlvl;
	char	*new_shlvl;

	if (!ht)
		return ;
	shlvl = ht_get(ht, "SHLVL");
	if (shlvl)
		new_shlvl = ft_itoa(ft_atoi(ht_get(ht, "SHLVL")) + 1);
	else
		new_shlvl = ft_itoa(1);
	if (!new_shlvl)
		print_error("malloc", EXIT_FAILURE);
	ht_insert(ht, "SHLVL", new_shlvl);
	free(new_shlvl);
}

static void	update_pwd(t_hash_table *ht)
{
	char	*cwd;

	if (!ht)
		return ;
	cwd = (char *)malloc(sizeof(char) * PATH_MAX);
	if (!cwd)
		print_error("malloc", EXIT_FAILURE);
	if (getcwd(cwd, PATH_MAX))
		ht_insert(ht, "PWD", cwd);
	else
	{
		free(cwd);
		print_error("malloc", EXIT_FAILURE);
	}
	free(cwd);
}

static void	ht_insert_env(t_hash_table *ht, const char **envp)
{
	char	*key;
	int		i;

	if (!ht)
		return ;
	i = 0;
	while (envp[i])
	{
		key = extract_key(envp[i]);
		if (!key)
			print_error("malloc", EXIT_FAILURE);
		ht_insert(ht, key, ft_strchr(envp[i], '=') + 1);
		free(key);
		++i;
	}
	ht_insert(ht, "_", "]");
	update_pwd(ht);
	update_shlvl(ht);
}

void	interactive_loop(const char **envp)
{
	t_hash_table	*ht;
	char			*input;

	ht = ht_create();
	if (!ht)
		print_error("malloc", EXIT_FAILURE);
	ht_insert_env(ht, envp);
	while (true)
	{
		input = readline("minishell$ ");
		if (!input)
		{
			move_cursor_up();
			tputs("minishell$ exit\n", 1, ft_putchar);
			break ;
		}
		add_history(input);
		free(input);
	}
	ht_destroy(ht);
	rl_clear_history();
}
