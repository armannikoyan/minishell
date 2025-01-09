#include "../../includes/minishell.h"

extern int	g_errno;

static void	handle_fork_and_execute(t_node *node, char **envp)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		handle_fork_error();
		return ;
	}
	if (pid == 0)
		execute_command_in_child(node, envp);
	else
	{
		signal(SIGINT, ft_parent_child_signal_handler);
		signal(SIGQUIT, SIG_IGN);
		waitpid(pid, &status, 0);
		signal(SIGINT, ft_signal_handler);
		signal(SIGQUIT, ft_signal_handler);
		if (WIFEXITED(status))
			g_errno = WEXITSTATUS(status);
		else
			g_errno = 1;
	}
}

void	ft_expand_error_code(t_node *node)
{
	unsigned short	i;
	if (!node)
		return ;
	i = 0;
	while (node->content[i])
	{
		if (ft_strcmp(node->content[i], "$?") == 0)
		{
			free(node->content[i]);
			node->content[i] = ft_itoa(g_errno);
		}
		i++;
	}
}

int	ft_mtx_strlen(char **mtx)
{
	int	i;

	i = 0;
	while (mtx[i])
		i++;
	return (i);
}

void	execute_command_in_child(t_node *node, char **envp)
{
	signal(SIGINT, ft_child_signal_handler);
	signal(SIGQUIT, SIG_IGN);
	if (ft_strcmp(node->content[0], "echo") == 0)
		g_errno = ft_echo(ft_mtx_strlen(node->content), node->content);
	else if (ft_strcmp(node->content[0], "env") == 0)
		g_errno = ft_env(ft_mtx_strlen(node->content), node->content, envp);
	else if (ft_strcmp(node->content[0], "pwd") == 0)
		g_errno = ft_pwd(ft_mtx_strlen(node->content), node->content);
	else if (execve(node->content[0], node->content, envp) == -1)
	{
		ft_report_error("command not found: ", node->content[0], 127);
		exit(g_errno);
	}
}

void	execute_command(t_node *node, char **envp)
{
	ft_expand_error_code(node);
	// printf("testing before EXPORTT");
	// for (int j = 0; envp[j] != NULL; j++)
	// {
	// 	printf("envp[%d] address: %p, value: %s\n", j, (void *)envp[j], envp[j]);
	// }
	if (ft_strcmp(node->content[0], "cd") == 0)
		g_errno = ft_cd(ft_mtx_strlen(node->content), node->content, envp);
	else if (ft_strcmp(node->content[0], "export") == 0)
		g_errno = ft_export(ft_mtx_strlen(node->content), node->content, &envp);
	else if (ft_strcmp(node->content[0], "unset") == 0)
		g_errno = ft_unset(ft_mtx_strlen(node->content), node->content, envp);
	else if (ft_strcmp(node->content[0], "exit") == 0)
		g_errno = ft_exit(ft_mtx_strlen(node->content), node->content);
	else
		handle_fork_and_execute(node, envp);
	// printf("testing after EXPORTT");
	// for (int j = 0; envp[j] != NULL; j++)
	// {
	// 	printf("envp[%d] address: %p, value: %s\n", j, (void *)envp[j], envp[j]);
	// }
}
