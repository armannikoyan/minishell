/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 16:57:05 by anikoyan          #+#    #+#             */
/*   Updated: 2024/09/24 15:01:25 by anikoyan         ###   ########.fr       */
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

static char	*ft_entry_info(void)
{
	char	*username;
	char	*w_dir;
	char	*temp;
	char	*prompt;

	username = NULL;
	w_dir = NULL;
	ft_entry_info_helper(&username, &w_dir);
	prompt = ft_strjoin(username, " ");
	free(username);
	temp = ft_strjoin(prompt, GREEN);
	free(prompt);
	prompt = ft_strjoin(temp, w_dir);
	free(temp);
	free(w_dir);
	temp = ft_strjoin(prompt, WHITE);
	free(prompt);
	prompt = ft_strjoin(temp, " % ");
	free(temp);
	return (prompt);
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	char	*prompt;

	(void)argc;
	(void)argv;
	(void)envp;
	ft_printf("%s ███▄ ▄███▓ ██▓ ███▄    █  ██▓  ██████  ██░ ██ ▓█████  ██▓     ██▓    %s\n", RED, WHITE);
	ft_printf("%s▓██▒▀█▀ ██▒▓██▒ ██ ▀█   █ ▓██▒▒██    ▒ ▓██░ ██▒▓█   ▀ ▓██▒    ▓██▒    %s\n", RED, WHITE);
	ft_printf("%s▓██    ▓██░▒██▒▓██  ▀█ ██▒▒██▒░ ▓██▄   ▒██▀▀██░▒███   ▒██░    ▒██░    %s\n", RED, WHITE);
	ft_printf("%s▒██    ▒██ ░██░▓██▒  ▐▌██▒░██░  ▒   ██▒░▓█ ░██ ▒▓█  ▄ ▒██░    ▒██░    %s\n", RED, WHITE);
	ft_printf("%s▒██▒   ░██▒░██░▒██░   ▓██░░██░▒██████▒▒░▓█▒░██▓░▒████▒░██████▒░██████▒%s\n", RED, WHITE);
	ft_printf("%s░ ▒░   ░  ░░▓  ░ ▒░   ▒ ▒ ░▓  ▒ ▒▓▒ ▒ ░ ▒ ░░▒░▒░░ ▒░ ░░ ▒░▓  ░░ ▒░▓  ░%s\n", RED, WHITE);
	ft_printf("%s░  ░      ░ ▒ ░░ ░░   ░ ▒░ ▒ ░░ ░▒  ░ ░ ▒ ░▒░ ░ ░ ░  ░░ ░ ▒  ░░ ░ ▒  ░%s\n", RED, WHITE);
	ft_printf("%s░      ░    ▒ ░   ░   ░ ░  ▒ ░░  ░  ░   ░  ░░ ░   ░     ░ ░     ░ ░   %s\n", RED, WHITE);
	ft_printf("%s       ░    ░           ░  ░        ░   ░  ░  ░   ░  ░    ░  ░    ░  ░%s\n\n", RED, WHITE);
	while (true)
	{
		prompt = ft_entry_info();
		input = readline(prompt);
		if (input && *input)
		{
			add_history(input);
			if (ft_has_syntax_error(input))
			{
				free(input);
				continue ;
			}
			if (!ft_strcmp(input, "exit"))
			{
				free(input);
				free(prompt);
				rl_clear_history(); //clear history on exit man
				return 0;
			}
			free(input);
		}
		free(prompt);
	}
}
