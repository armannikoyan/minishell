/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvarnach <lvarnach@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 19:44:43 by lvarnach          #+#    #+#             */
/*   Updated: 2026/01/28 19:44:44 by lvarnach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "execution.h"
#include "expansion.h"
#include "utils.h"
#include "../libs/libft/libft.h"

int	expand_argv_vars(t_ast_node *cmd, t_hash_table *ht, int errnum)
{
	size_t	i;
	char	*tmp;

	i = 0;
	while (cmd->u_data.cmd.argv[i])
	{
		tmp = cmd->u_data.cmd.argv[i];
		cmd->u_data.cmd.argv[i] = expand_dollar_sign(tmp, ht, errnum);
		free(tmp);
		if (!cmd->u_data.cmd.argv[i])
			return (print_error("minishell: malloc", false), 1);
		i++;
	}
	return (0);
}

int	run_expansion(t_ast_node *node, t_hash_table *ht, int errnum)
{
	char	**new_argv;

	if (expand_argv_vars(node, ht, errnum))
		return (1);
	new_argv = expand_wildcards(node->u_data.cmd.argv);
	if (!new_argv)
		return (print_error("minishell: malloc", false), 1);
	free_split(node->u_data.cmd.argv);
	node->u_data.cmd.argv = new_argv;
	return (0);
}

int	check_basic_builtins(char *cmd, char **av, t_hash_table *ht)
{
	int	ac;

	ac = 0;
	while (av[ac])
		ac++;
	if (ft_strcmp(cmd, "echo") == 0)
		return (update_underscore(ht, cmd), ft_echo(ac, av, ht));
	if (ft_strcmp(cmd, "cd") == 0)
		return (update_underscore(ht, cmd), ft_cd(ac, av, ht));
	if (ft_strcmp(cmd, "pwd") == 0)
		return (update_underscore(ht, cmd), ft_pwd(ac, av, ht));
	if (ft_strcmp(cmd, "export") == 0)
		return (update_underscore(ht, cmd), ft_export(ac, av, ht));
	return (-1);
}

int	check_extra_builtins(char **av, t_hash_table *ht, int e, t_ast_node *root)
{
	int	ac;

	ac = 0;
	while (av[ac])
		ac++;
	if (ft_strcmp(av[0], "unset") == 0)
		return (update_underscore(ht, av[0]), ft_unset(ac, av, ht));
	if (ft_strcmp(av[0], "env") == 0)
		return (update_underscore(ht, av[0]), ft_env(ac, av, ht));
	if (ft_strcmp(av[0], "exit") == 0)
		return (update_underscore(ht, av[0]), ft_exit(av, ht, e, root));
	return (-1);
}

int	run_builtin(char **argv, t_hash_table *ht, int errnum, t_ast_node *root)
{
	int	res;

	if (!argv || !argv[0])
		return (false);
	res = check_basic_builtins(argv[0], argv, ht);
	if (res != -1)
		return (res);
	res = check_extra_builtins(argv, ht, errnum, root);
	return (res);
}
