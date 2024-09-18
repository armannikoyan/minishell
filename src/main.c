/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 16:57:05 by anikoyan          #+#    #+#             */
/*   Updated: 2024/09/18 20:55:18 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*ft_sub_user(char *abs_wdir)
{
	unsigned int	len;
	char			*username;
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
	username = (char *)malloc(len + 1);
	if (!username)
		exit(-1);
	return (ft_substr(abs_wdir, i, len));
}

static char	*ft_sub_wdir(char *abs_wdir)
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

static void	ft_entry_info(char **username, char **w_dir)
{
	char	*abs_wdir;

	(void)username;
	(void)w_dir;
	abs_wdir = NULL;
	abs_wdir = getcwd(abs_wdir, MAXPATHLEN);
	*username = ft_sub_user(abs_wdir);
	*w_dir = ft_sub_wdir(abs_wdir);
	free(abs_wdir);
}

static char	*ft_build_prompt(void)
{
	char	*username;
	char	*w_dir;
	char	*temp;
	char	*prompt;

	username = NULL;
	w_dir = NULL;
	ft_entry_info(&username, &w_dir);
	prompt = ft_strjoin(GREEN, username);
	free(username);
	temp = ft_strjoin(prompt, WHITE);
	free(prompt);
	prompt = ft_strjoin(temp, " ");
	free(temp);
	temp = ft_strjoin(prompt, w_dir);
	free(w_dir);
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
	while (true)
	{
		prompt = ft_build_prompt();
		input = readline(prompt);
		if (input && *input)
		{
			add_history(input);
			// ft_process_input(input, envp);
		}
		free(input);
		free(prompt);
	}
}
