/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_command_node.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 01:31:06 by lvarnach          #+#    #+#             */
/*   Updated: 2026/01/28 18:10:01 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#include "ast.h"
#include "builtin.h"
#include "error_codes.h"
#include "execution.h"
#include "hash_table.h"
#include "expansion.h"
#include "term_settings.h"
#include "utils.h"
#include "path_utils.h"
#include "../../libs/libft/libft.h"

static int	handle_exec_error(char *path, int err_code)
{
	print_error("minishell: ", true);
	print_error(path, true);
	if (err_code == COMMAND_NOT_FOUND)
		print_error(": command not found\n", true);
	else if (err_code == IS_A_DIRECTORY)
		print_error(": Is a directory\n", true);
	else if (err_code == PERMISSION_DENIED)
		print_error(": Permission denied\n", true);
	return (err_code);
}

static int	validate_executable(char *path)
{
	size_t	mask;

	mask = check_access(path);
	if (!(mask & EXISTS_BIT))
		return (handle_exec_error(path, COMMAND_NOT_FOUND));
	if (!(mask & FILE_BIT))
		return (handle_exec_error(path, IS_A_DIRECTORY));
	if (!(mask & EXEC_BIT))
		return (handle_exec_error(path, PERMISSION_DENIED));
	return (0);
}

static int	handle_child_exec(char *cmd_path, char **argv, t_garbage *g)
{
	char	**envp;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	envp = ht_to_envp(g->ht);
	if (!envp)
	{
		clean_all_stacks(g);
		ast_deletion(g->root);
		ht_destroy(g->ht);
		exit(EXIT_FAILURE);
	}
	if (execve(cmd_path, argv, envp) == -1)
	{
		print_error("minishell: ", true);
		print_error(argv[0], true);
		print_error(": ", false);
		free_split(envp);
		clean_all_stacks(g);
		ast_deletion(g->root);
		ht_destroy(g->ht);
		if (errno == ENOENT)
			exit(COMMAND_NOT_FOUND);
		if (errno == EACCES)
			exit(PERMISSION_DENIED);
		if (errno == EISDIR)
			exit(IS_A_DIRECTORY);
		exit(1);
	}
	return (0);
}

int	execute_command(t_ast_node *node, int errnum, t_garbage *g)
{
	char	*path;
	int		code;
	pid_t	pid;

	if (run_expansion(node, g->ht, errnum) != 0)
		return (1);
	code = run_builtin(node->u_data.cmd.argv, g, errnum);
	if (code != -1)
		return (code);

	path = resolve_cmd_path(node->u_data.cmd.argv[0], g->ht);
	update_underscore(g->ht, path);
	code = validate_executable(path);
	if (code != 0)
		return (free(path), code);
	pid = fork();
	if (pid == -1)
		return (free(path), print_error("minishell: fork", false), 1);
	if (pid == 0)
		handle_child_exec(path, node->u_data.cmd.argv, g);
	free(path);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	code = handle_child_exit(pid);
	psig_set();
	return (code);
}