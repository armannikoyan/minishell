/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:04:00 by gsimonia          #+#    #+#             */
/*   Updated: 2025/01/07 22:29:26 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_errno;

// BUG: cannot use cd ~: cd: Operation not permitted~
static char	*get_env_path(char **envp, const char *var, size_t len)
{
	while (*envp)
	{
		if (ft_strncmp(*envp, var, len) == 0 && (*envp)[len] == '=')
			return (ft_strdup(*envp + len));
		envp++;
	}
	return (NULL);
}

static int	update_oldpwd(char **envp)
{
	char	cwd[PATH_MAX];
	char	*oldpwd;
	size_t	i;

	i = 0;
	if (getcwd(cwd, PATH_MAX) == NULL)
		return (write_error("cd: ", strerror(g_errno), NULL));
	oldpwd = (char *)malloc(ft_strlen("OLDPWD=") + ft_strlen(cwd) + 1);
	if (!oldpwd)
		return (write_error("cd: ", "memory allocation failed\n", NULL));
	ft_strcpy(oldpwd, "OLDPWD=");
	ft_strcat(oldpwd, cwd);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "OLDPWD=", 7) == 0)
		{
			free(envp[i]);
			envp[i] = oldpwd;
			return (EXIT_SUCCESS);
		}
		i++;
	}
	envp[i] = oldpwd;
	envp[i + 1] = NULL;
	return (EXIT_SUCCESS);
}

static int	go_to_path(int option, char **envp)
{
	char	*env_path;

	env_path = NULL;
	if (option == 0)
		env_path = get_env_path(envp, "HOME=", 5);
	else if (option == 1)
		env_path = get_env_path(envp, "OLDPWD=", 7);
	if (!env_path)
	{
		if (option == 0)
			return (write_error(NULL, "cd: HOME not set\n", NULL));
		return (write_error(NULL, "cd: OLDPWD not set\n", NULL));
	}
	if (update_oldpwd(envp) == EXIT_FAILURE)
	{
		free(env_path);
		return (write_error("cd: ", "failed to update OLDPWD\n", NULL));
	}
	if (chdir(env_path) < 0)
	{
		free(env_path);
		return (write_error("cd: ", strerror(g_errno), NULL));
	}
	free(env_path);
	return (EXIT_SUCCESS);
}

int	ft_cd(int argc, char **argv, char **envp)
{
	int	cd_ret;

	if (argc > 2)
		return (write_error("cd: ", "too many arguments\n", NULL));
	if (argc < 2 || !argv[1])
		return (go_to_path(0, envp));
	if (ft_strcmp(argv[1], "-") == 0)
		return (go_to_path(1, envp));
	if (update_oldpwd(envp) == EXIT_FAILURE)
		return (write_error("cd: ", "failed to update OLDPWD\n", NULL));
	cd_ret = chdir(argv[1]);
	if (cd_ret < 0)
		return (write_error("cd: ", strerror(g_errno), argv[1]));
	return (EXIT_SUCCESS);
}
