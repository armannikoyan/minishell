#include "../../includes/minishell.h"

extern int	g_errno;

static int	ft_open_file_for_input_redirection(const char *filepath)
{
	int	fd;

	fd = open(filepath, O_RDONLY);
	if (fd == -1)
	{
		perror("open failed");
		g_errno = 1;
	}
	return (fd);
}

void	ft_handle_input_redirection(t_node *node, char **envp)
{
	int		fd;
	pid_t	pid;

	if (!node || !node->left || !node->content[1])
		return ;
	fd = ft_open_file_for_input_redirection(node->content[1]);
	if (fd == -1)
		return ;
	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
		ft_execute_input_child_process(node, envp, fd);
	else
	{
		close(fd);
		waitpid(pid, NULL, 0);
	}
}

static int	ft_open_file_for_redirection(const char *filepath, int flags)
{
	int	fd;

	fd = open(filepath, O_WRONLY | O_CREAT | flags, 0644);
	if (fd == -1)
	{
		perror("open failed");
		g_errno = 1;
	}
	return (fd);
}

void	ft_handle_output_redirection(t_node *node, char **envp, int flags)
{
	int		fd;
	pid_t	pid;

	if (!node || !node->left)
		return ;
	fd = ft_open_file_for_redirection(node->content[1], flags);
	if (fd == -1)
		return ;
	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
		ft_execute_child_process(node, envp, fd);
	else
	{
		close(fd);
		waitpid(pid, NULL, 0);
	}
}

void	ft_execute_input_child_process(t_node *node, char **envp, int fd)
{
	dup2(fd, STDIN_FILENO);
	close(fd);
	ft_exec(&(t_tree){node->left}, envp);
	exit(g_errno);
}
