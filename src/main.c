/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 16:57:05 by anikoyan          #+#    #+#             */
/*   Updated: 2024/12/25 19:10:21 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_errno;

extern void	rl_clear_history(void);

static char	*ft_subusr(char *abs_wdir)
{
	unsigned int	len;
	short			slash_count;
	size_t			i;

	slash_count = 0;
	i = 0;
	while (slash_count != 2)
	{
		if (abs_wdir[i] == '/')
			slash_count++;
		i++;
	}
	len = 0;
	while ((&(abs_wdir[i]))[len] != '/')
		len++;
	return (ft_substr(abs_wdir, i, len));
}

static char	*ft_subwdir(char *abs_wdir)
{
	unsigned int	len;
	size_t			i;

	i = 0;
	while (abs_wdir[i])
		i++;
	len = 0;
	while (abs_wdir[i - 1] != '/')
	{
		i--;
		len++;
	}
	return (ft_substr(abs_wdir, i, len));
}

static void	ft_entry_info_helper(char **username, char **w_dir)
{
	char	*abs_wdir;

	(void)username;
	(void)w_dir;
	abs_wdir = NULL;
	abs_wdir = getcwd(abs_wdir, MAXPATHLEN);
	*username = ft_subusr(abs_wdir);
	*w_dir = ft_subwdir(abs_wdir);
	free(abs_wdir);
}

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

void printf_list(t_list *lst)
{
	t_list *tmp;

	tmp = lst;
	while (tmp)
	{
		ft_printf("--------------\n");
		ft_printf("content: %s\n", ((t_token *)tmp->content)->content);
		ft_printf("type: %c\n", ((t_token *)tmp->content)->type);
		ft_printf("subshell_level: %d\n", ((t_token *)tmp->content)->subshell_level);
		ft_printf("--------------\n");
		tmp = tmp->next;
	}
}

static void	handle_input(char *input, char **envp, t_list **lst, t_tree **tree)
{
	char	*tmp;

	tmp = input;
	input = ft_space_correction(tmp);
	free(tmp);
	tmp = input;
	input = ft_env_expansion(tmp, envp);
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
	printf_list(*lst);
	(void)tree;
	// *tree = ft_tree_build(lst);
	// if (!*tree)
	// 	exit(EXIT_FAILURE);
	ft_lstclear(lst, ft_tokendelone);
	free(lst);
	// ft_exec(*tree, envp);
}

static void	run_shell_loop(char **envp)
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

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	signal(SIGINT, ft_signal_handler);
	run_shell_loop(envp);
}
