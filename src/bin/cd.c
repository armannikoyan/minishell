/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:04:00 by gsimonia          #+#    #+#             */
/*   Updated: 2025/01/13 20:05:19 by gsimonia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_errno;

static char	*get_env_path(char **envp, const char *var, size_t len)
{
	while (*envp)
	{
		if (ft_strncmp(*envp, var, len) == 0 && (*envp)[len] == '=')
			return (ft_strdup(*envp + len + 1));
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
		return (ft_report_error("cd", strerror(g_errno), 1));
	oldpwd = (char *)malloc(ft_strlen("OLDPWD=") + ft_strlen(cwd) + 1);
	if (!oldpwd)
		return (ft_report_error("cd: ", "memory allocation failed", 1));
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
		env_path = get_env_path(envp, "HOME", 4);
	else if (option == 1)
		env_path = get_env_path(envp, "OLDPWD", 6);
	if (!env_path)
	{
		if (option == 0)
			return (ft_report_error("cd: ", "HOME not set", 1));
		return (ft_report_error("cd: ", "OLDPWD not set", 1));
	}
	if (update_oldpwd(envp) != EXIT_SUCCESS)
	{
		free(env_path);
		return (EXIT_FAILURE);
	}
	if (chdir(env_path) < 0)
	{
		free(env_path);
		return (ft_report_error("cd: ", "unknown error occurred", 1));
	}
	free(env_path);
	return (EXIT_SUCCESS);
}

static int	expand_home_and_cd(const char *ang, char **envp)
{
	char	*home;
	char	*expanded_path;
	int		cd_ret;

	home = get_env_path(envp, "HOME", 4);
	if (!home)
		return (ft_report_error("cd: ", "HOME not set", 1));
	expanded_path = (char *)malloc(ft_strlen(home)
			+ ft_strlen(ang));
	if (!expanded_path)
	{
		free(home);
		return (ft_report_error("cd: ", "memory allocation failed", 1));
	}
	ft_strcpy(expanded_path, home);
	ft_strcat(expanded_path, ang + 1);
	free(home);
	cd_ret = chdir(expanded_path);
	free(expanded_path);
	if (cd_ret < 0)
	{
		ft_report_error_arg("cd: ", ": No such file or directory", 1, ang);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	ft_cd(int argc, char **argv, char **envp)
{
	int		cd_ret;

	if (argc < 2 || !argv[1])
		return (go_to_path(0, envp));
	if (ft_strcmp(argv[1], "-") == 0)
		return (go_to_path(1, envp));
	if (argv[1][0] == '~')
	{
		cd_ret = expand_home_and_cd(argv[1], envp);
		if (cd_ret != EXIT_SUCCESS)
			return (EXIT_FAILURE);
		return (EXIT_SUCCESS);
	}
	if (update_oldpwd(envp) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	cd_ret = chdir(argv[1]);
	if (cd_ret < 0)
	{
		ft_report_error_arg("cd: ", ": No such file or directory", 1, argv[1]);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
