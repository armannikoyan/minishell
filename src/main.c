/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 16:57:05 by anikoyan          #+#    #+#             */
/*   Updated: 2024/11/20 17:31:05 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_errno;

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

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	char	*tmp;
	char	*prompt;
	t_list	**lst;

	(void)argc;
	(void)argv;
	signal(SIGINT, ft_signal_handler);
	while (true)
	{
		prompt = ft_entry_info();
		input = readline(prompt);
		if (input && *input)
		{
			add_history(input);
			tmp = input;
			input = ft_space_correction(tmp);
			free(tmp);
			tmp = input;
			input = ft_env_expansion(tmp, envp);
			free(tmp);
			lst = ft_tokenization(input);
			free(input);
			if (lst)
				ft_has_syntax_error(lst);
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
