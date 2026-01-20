#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#include "ast.h"
#include "builtin.h"
#include "execution.h"
#include "hash_table.h"
#include "expansion.h"
#include "term_settings.h"
#include "utils.h"
#include "path_utils.h"
#include "../../libs/libft/libft.h"

static void update_underscore(t_hash_table *ht, char *value)
{
    if (ht_get(ht, "_") == NULL)
        ht_create_bucket(ht, "_", value, false);
    else
        ht_update_value(ht, "_", value);
}

static char **ht_to_envp(t_hash_table *ht)
{
    char    **envp;
    t_entry *entry;
    char    *tmp;
    size_t  i;
    size_t  j;

    envp = malloc(sizeof(char *) * (ht->count + 1));
    if (!envp)
        return (NULL);
    i = 0;
    j = 0;
    while (i < (size_t)ht->size)
    {
        entry = ht->buckets[i];
        while (entry)
        {
            tmp = ft_strjoin(entry->key, "=");
            if (tmp)
            {
                envp[j] = ft_strjoin(tmp, entry->val);
                free(tmp);
                if (envp[j])
                    j++;
            }
            entry = entry->next;
        }
        i++;
    }
    envp[j] = NULL;
    return (envp);
}

static char *search_in_path_env(char *cmd, t_hash_table *ht)
{
    char    **paths;
    char    *full_path;
    char    *tmp;
    int     i;

    if (!ht_get(ht, "PATH"))
        return (NULL);
    paths = ft_split(ht_get(ht, "PATH")->val, ':');
    if (!paths)
        return (NULL);
    i = -1;
    full_path = NULL;
    while (paths[++i])
    {
        tmp = ft_strjoin(paths[i], "/");
        full_path = ft_strjoin(tmp, cmd);
        free(tmp);
        if (access(full_path, F_OK) == 0)
            break ;
        free(full_path);
        full_path = NULL;
    }
    free_split(paths);
    return (full_path);
}

static char *resolve_cmd_path(char *cmd, t_hash_table *ht)
{
    char    *cmd_path;

    if (!cmd || !*cmd)
        return (NULL);
    if (ft_strchr(cmd, '/'))
        return (ft_strdup(cmd));
    cmd_path = search_in_path_env(cmd, ht);
    if (!cmd_path)
        return (ft_strdup(cmd));
    return (cmd_path);
}

static int run_expansion(t_ast_node *node, t_hash_table *ht)
{
    size_t  i;
    char    *tmp;

    i = 0;
    while (node->u_data.cmd.argv[i])
    {
        tmp = node->u_data.cmd.argv[i];
        node->u_data.cmd.argv[i] = expand_dollar_sign(tmp, ht);
        free(tmp);
        if (!node->u_data.cmd.argv[i])
            return (print_error("minishell: malloc", false), 1);

        tmp = node->u_data.cmd.argv[i];
        node->u_data.cmd.argv[i] = remove_quotes(tmp);
        free(tmp);
        if (!node->u_data.cmd.argv[i])
            return (print_error("minishell: malloc", false), 1);
        i++;
    }
    return (0);
}

static int count_args(char **argv)
{
    int i;

    i = 0;
    while (argv[i])
        i++;
    return (i);
}

static bool run_builtin(char **argv, t_hash_table *ht)
{
    int     argc;
    int     status;
    char    *cmd;
    bool    is_builtin;

    if (!argv || !argv[0])
        return (false);

    cmd = argv[0];
    argc = count_args(argv);
    status = 0;
    is_builtin = true;

    if (ft_strcmp(cmd, "echo") == 0)
    {
        update_underscore(ht, cmd);
        status = ft_echo(argc, argv, ht);
    }
    else if (ft_strcmp(cmd, "cd") == 0)
    {
        update_underscore(ht, cmd);
        status = ft_cd(argc, argv, ht);
    }
    else if (ft_strcmp(cmd, "pwd") == 0)
    {
        update_underscore(ht, cmd);
        status = ft_pwd(argc, argv, ht);
    }
    // else if (ft_strcmp(cmd, "export") == 0)
    // {
    //     update_underscore(ht, cmd);
    //     status = ft_export(argc, argv, ht);
    // }
    // else if (ft_strcmp(cmd, "unset") == 0)
    // {
    //     update_underscore(ht, cmd);
    //     status = ft_unset(argc, argv, ht);
    // }
    else if (ft_strcmp(cmd, "env") == 0)
    {
        update_underscore(ht, cmd);
        status = ft_env(argc, argv, ht);
    }
    else if (ft_strcmp(cmd, "exit") == 0)
    {
        update_underscore(ht, cmd);
        status = ft_exit(argc, argv, ht);
    }
    else
        is_builtin = false;

    if (is_builtin)
        errno = status;

    return (is_builtin);
}

static int validate_executable(char *path)
{
    size_t  mask;

    mask = check_access(path);

    if (!(mask & EXISTS_BIT))
    {
        print_error("minishell: ", true);
        print_error(path, true);
        print_error(": command not found\n", true);
        return (127);
    }
    if (!(mask & FILE_BIT))
    {
        print_error("minishell: ", true);
        print_error(path, true);
        print_error(": Is a directory\n", true);
        return (126);
    }
    if (!(mask & EXEC_BIT))
    {
        print_error("minishell: ", true);
        print_error(path, true);
        print_error(": Permission denied\n", true);
        return (126);
    }
    return (0);
}

static void execute_child(char *cmd_path, char **argv, t_hash_table *ht)
{
    char    **envp;

    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);

    envp = ht_to_envp(ht);
    if (!envp)
        exit(EXIT_FAILURE);

    if (execve(cmd_path, argv, envp) == -1)
    {
        print_error("minishell: ", true);
        print_error(argv[0], true);
        print_error(": ", false);
        free_split(envp);

        if (errno == ENOENT)
            exit(127);
        else if (errno == EACCES)
            exit(126);
        else
            exit(1);
    }
}

void execute_command(t_ast_node *node, t_hash_table *ht)
{
    char    *cmd_path;
    int     err_code;
    pid_t   pid;

    // 1. Expansion
    if (run_expansion(node, ht) != 0)
        return ;

    if (run_builtin(node->u_data.cmd.argv, ht))
        return ;

    cmd_path = resolve_cmd_path(node->u_data.cmd.argv[0], ht);
    if (!cmd_path)
        return ;

    update_underscore(ht, cmd_path);

    err_code = validate_executable(cmd_path);
    if (err_code != 0)
    {
        errno = err_code;
        free(cmd_path);
        return ;
    }

    pid = fork();
    if (pid == -1)
        perror("minishell: fork");
    else if (pid == 0)
    {
        execute_child(cmd_path, node->u_data.cmd.argv, ht);
    }
    else
    {
        free(cmd_path);
        signal(SIGINT, SIG_IGN);
        signal(SIGQUIT, SIG_IGN);
        handle_child_exit(pid);
        psig_set();
    }
}