#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#include "ast.h"
#include "execution.h"
#include "hash_table.h"
#include "expansion.h"
#include "term_settings.h"
#include "utils.h"
#include "../../libs/libft/libft.h"

static char    *get_cmd_path(char *cmd, size_t *i, size_t *access_bitmask, t_hash_table *ht) {
    char    *cmd_path;
    char    **path;
    char    *tmp;

    cmd_path = NULL;
    if (cmd[*i] != '/') {
        path = split_env_var(ht_get(ht, "PATH"));
        *i = 0;
        while (path[*i]) {
            tmp = ft_strjoin(path[*i], "/");
            if (tmp == NULL) {
                // TODO: write normal error
                perror("malloc");
            }
            cmd_path = ft_strjoin(tmp, cmd);
            free(tmp);
            if (cmd_path == NULL) {
                // TODO: write normal error
                perror("malloc");
            }
            *access_bitmask = check_access(cmd_path);
            if (*access_bitmask & EXISTS_BIT)
                break ;
            ++(*i);
            free(cmd_path);
        }
        free_split(path);
    }
    if (!cmd_path)
    {
        cmd_path = cmd;
        *access_bitmask = check_access(cmd_path);
    }
    return (cmd_path);
}

void    execute_command(t_ast_node *node, t_hash_table *ht)
{
    char    *cmd;
    char    *cmd_path;
    size_t  access_bitmask;
    size_t  i;

    i = -1;
    while (node->u_data.cmd.argv[++i])
    {
        node->u_data.cmd.argv[i] = expand_dollar_sign(node->u_data.cmd.argv[i], ht);
        if (node->u_data.cmd.argv[i] == NULL) {
            print_error("minishell: malloc", false);
            // TODO: Destruct everything or return error status so that the execution stops and the tree destroys
            return ;
        }
        // TODO: implementation of the expansion of wildcard and tilde
        // TODO: remove quotes
    }
    i = 0;
    cmd = node->u_data.cmd.argv[0];
    while (cmd[i]) {
        if (cmd[i] == '/')
            break ;
        ++i;
    }
    access_bitmask = 0;
    cmd_path = get_cmd_path(cmd, &i, &access_bitmask, ht);
    if (access_bitmask & EXISTS_BIT) {
        if (!(access_bitmask & STAT_FAIL_BIT)) {
            if (access_bitmask & FILE_BIT) {
                if (access_bitmask & EXEC_BIT) {
                    pid_t pid;

                    ht_insert(ht, "_", cmd_path);
                    pid = fork();
                    if (pid == -1) {
                        // TODO: write normal error
                        perror("fork");
                        return ;
                    }
                    if (pid) {
                        signal(SIGINT, SIG_IGN);
                        signal(SIGQUIT, SIG_IGN);
                        handle_child_exit(pid);
                        printf("child exited with code %d\n", pid);
                        psig_set();
                    }
                    else {
                        char **envp;
                        size_t j;
                        envp = (char **)malloc(sizeof(char *) * (ht->count + 1));
                        if (envp == NULL) {
                            // TODO: write normal error
                            perror("malloc");
                        }
                        t_entry *entry;
                        i = 0;
                        j = 0;
                        while (i < (size_t)ht->size) {
                            entry = ht->buckets[i];
                            while (entry) {
                                char *tmp = ft_strjoin(entry->key, "=");
                                envp[j] = ft_strjoin(tmp, entry->value);
                                free(tmp);
                                entry = entry->next;
                            }
                            i++;
                        }
                        if (execve(cmd_path, node->u_data.cmd.argv, envp) == -1) {
                            exit(126);
                            free_split(envp);
                        }
                        free_split(envp);
                        exit(errno);
                    }
                }
            }
        }
    }
}