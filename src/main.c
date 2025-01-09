/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 16:57:05 by anikoyan          #+#    #+#             */
/*   Updated: 2025/01/09 10:17:16 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_errno;

extern void	rl_clear_history(void);

char	*ft_entry_info(void)
{
	char	*username;
	char	*w_dir;
	char	*prompt;
	char	*tmp;

	username = NULL;
	ft_entry_info_helper(&username, &w_dir);
	prompt = ft_strjoin("", username);
	free(username);
	tmp = ft_strjoin(prompt, " ");
	free(prompt);
	prompt = ft_strjoin(tmp, "\001" GREEN "\002");
	free(tmp);
	tmp = ft_strjoin(prompt, w_dir);
	free(prompt);
	free(w_dir);
	prompt = ft_strjoin(tmp, "\001" WHITE "\002 % ");
	free(tmp);
	return (prompt);
}

static void	handle_input(char *input, char ***envp, t_list **lst, t_tree **tree)
{
	char	*tmp;

	tmp = input;
	input = ft_space_correction(tmp);
	free(tmp);
	tmp = input;
	input = ft_env_expansion(tmp, *envp);
	free(tmp);
	lst = ft_tokenization(input);
	free(input);
	if (!lst)
		exit(EXIT_FAILURE);
	if (ft_has_syntax_error(lst))
	{
		ft_lstclear(lst, ft_tokendelone);
		free(lst);
		return ;
	}
	*tree = ft_tree_build(lst);
	if (!*tree)
		exit(EXIT_FAILURE);
	ft_lstclear(lst, ft_tokendelone);
	free(lst);
	ft_exec(*tree, envp);
}

static void	run_shell_loop(char ***envp)
{
	char	*input;
	char	*prompt;
	t_list	**lst;
	t_tree	*tree;

	lst = NULL;
	tree = NULL;
	while (true)
	{
		prompt = ft_entry_info();
		input = readline(prompt);
		if (input && *input)
		{
			add_history(input);
			handle_input(input, envp, lst, &tree);
			ft_tree_dtor(&tree);
		}
		else if (!input)
		{
			free(prompt);
			rl_clear_history();
			exit(EXIT_SUCCESS);
		}
		free(prompt);
	}
}

char	**ft_copy_envp(char **envp)
{
	char			**envp_cpy;
	char			*tmp;
	unsigned int	i;

	envp_cpy = (char **)malloc(sizeof(char *) * (ft_mtx_strlen(envp) + 1));
	if (!envp_cpy)
		exit(EXIT_FAILURE);
	i = -1;
	while (envp[++i])
	{
		if (ft_strncmp(envp[i], "SHELL=", 6) == 0)
			envp_cpy[i] = ft_strdup("SHELL=minishell");
		else if (ft_strncmp(envp[i], "SHLVL=", 6) == 0)
		{
			tmp = ft_itoa(ft_atoi(envp[i] + 6) + 1);
			envp_cpy[i] = ft_strjoin("SHLVL=", tmp);
			free(tmp);
		}
		else
			envp_cpy[i] = ft_strdup(envp[i]);
		if (!envp_cpy[i])
			exit(EXIT_FAILURE);
	}
	envp_cpy[i] = NULL;
	return (envp_cpy);
}

int	main(int argc, char **argv, char **envp)
{
	char			**envp_cpy;

	(void)argc;
	(void)argv;
	signal(SIGINT, ft_signal_handler);
	signal(SIGQUIT, ft_signal_handler);
	envp_cpy = ft_copy_envp(envp);
	run_shell_loop(&envp_cpy);
}
