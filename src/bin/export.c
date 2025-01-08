/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsimonia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:06:01 by gsimonia          #+#    #+#             */
/*   Updated: 2025/01/06 18:33:50 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// static char **create_new_envp(char **envp)
// {
//     int i = 0;
//     char **new_envp;
//     int j;

//     printf("create_new_envp\n");
//     while (envp[i]) {
//         i++;
//     }
//     printf("Number of environment variables: %d\n", i);

//     new_envp = malloc(sizeof(char *) * (i + 1));  // Allocate space for i + 1 (NULL terminator)
//     if (!new_envp) {
//         printf("Memory allocation failed for new_envp\n");
//         return NULL;
//     }
//     printf("create_new_envp2\n");

//     for (j = 0; j < i; j++) {
//         printf("envp[%d] address: %p, value: %s\n", j, (void *)envp[j], envp[j]);
//         new_envp[j] = ft_strdup(envp[j]);
//         if (!new_envp[j]) {
//             printf("Memory allocation failed for new_envp[%d]\n", j);
//             while (--j >= 0) {
//                 free(new_envp[j]);
//             }
//             free(new_envp);
//             return NULL;
//         }
//         printf("new_envp[%d] address: %p, value: %s\n", j, (void *)new_envp[j], new_envp[j]);
//     }

//     new_envp[i] = NULL;  // Null-terminate the new envp array
//     printf("create_new_envp4\n");

//     return new_envp;
// }



// static int	set_new_var(char **new_envp, char *key, char *value, int index)
// {
// 	char	*new_var;

// 	printf("setnewvar\n");
// 	if (!key || !value)
// 	{
//     	write_error("export", "key or value is NULL\n", NULL);
//     	return (EXIT_FAILURE);
// 	}
// 	new_var = malloc(ft_strlen(key) + ft_strlen(value) + 2);
// 	if (!new_var)
//     	return (write_error("export: ", "memory allocation failed\n", NULL));
// 	ft_strcpy(new_var, key);
// 	ft_strcat(new_var, "=");
// 	ft_strcat(new_var, value);
// 	new_envp[index] = new_var;
// 	printf("new_envp[%d] address: %p, value: %s\n", index, (void *)new_envp[index], new_envp[index]);
// 	index++;
// 	printf("new_envp[%d] address: %p, value: %s\n", index, (void *)new_envp[index], new_envp[index]);
// 	return (EXIT_SUCCESS);
// }

// static int	set_env_var(char ***envp, const char *key, const char *value)
// {
// 	int		i;
// 	char	**new_envp;
// 	int		index;

// 	printf("set_env_var\n");
	
// 	// Create new environment array
// 	new_envp = create_new_envp(*envp);
// 	if (!new_envp)
// 		return (EXIT_FAILURE);
	
// 	// Calculate the index of the new variable
// 	i = 0;
// 	while ((*envp)[i])
// 		i++;
// 	index = i;

// 	// Attempt to set the new variable
// 	if (set_new_var(new_envp, (char *)key, (char *)value, index) != EXIT_SUCCESS)
// 	{
// 		// Free new_envp if setting the variable failed
// 		for (int k = 0; k < index; k++)
// 			free(new_envp[k]);
// 		free(new_envp);
// 		return (EXIT_FAILURE);
// 	}

// 	// Free the old environment and assign the new one
// 	if (*envp)
// 	{
// 		for (int k = 0; (*envp)[k]; k++)
// 			free((*envp)[k]);
// 		free(*envp);
// 	}
	
// 	// Assign the new environment array to *envp
// 	*envp = new_envp;
//     printf("Updated environment:\n");
//     for (int j = 0; (*envp)[j] != NULL; j++)
//     {
//         printf("envp[%d] address: %p, value: %s\n", j, (void *)(*envp)[j], (*envp)[j]);
//     }
// 	return (EXIT_SUCCESS);
// }


// static void	print_sorted_env(char **envp)
// {
// 	int		i;
// 	int		j;
// 	char	*tmp;

// 	i = 0;
// 	while (envp[i])
// 	{
// 		j = i + 1;
// 		while (envp[j])
// 		{
//     		if (envp[i] && envp[j] && ft_strcmp(envp[i], envp[j]) > 0)
//     		{
//         		tmp = envp[i];
//         		envp[i] = envp[j];
//         		envp[j] = tmp;
//     		}
//     		j++;
// 		}
// 		i++;
// 	}
// 	i = 0;
// 	while (envp[i])
// 		write_string("declare -x ", envp[i++], "\n");
// 	return ;
// }

// int	ft_export(int argc, char **argv, char **envp)
// {
// 	int		i;
// 	char	*key;
// 	char	*value;
// 	char	*save_ptr;

// 	printf("ft_export\n");
// 	for (int j = 0; envp[j] != NULL; j++)
//     {
//         printf("envp[%d] address: %p, value: %s\n", j, (void *)envp[j], envp[j]);
//     }
// 	(void)argc;
// 	i = 1;
// 	if (!argv[i])
// 	{
// 		print_sorted_env(envp);
// 		return (EXIT_SUCCESS);
// 	}
// 	while (argv[i])
// 	{
//     	if (!is_valid_env_var_key(argv[i]))
//         	return (write_error("export", argv[i], "not a valid identifier\n"));
//     	else if (ft_strchr(argv[i], '=') != NULL)
//     	{
//         	char *arg_copy = ft_strdup(argv[i]);
//         	if (!arg_copy)
//             	return (write_error("export", "memory allocation failed\n", NULL));
//         	key = ft_strtok(arg_copy, "=", &save_ptr);
//         	value = ft_strtok(NULL, "=", &save_ptr);
//         	if (set_env_var(&envp, key, value) != EXIT_SUCCESS)
//         	{
//             	free(arg_copy);
//             	return (EXIT_FAILURE);
//         	}
// 			printf("Updated environment after setting %s=%s:\n", key, value);
//             for (int j = 0; envp[j] != NULL; j++)
//             {
//                 printf("envp[%d] address: %p, value: %s\n", j, (void *)envp[j], envp[j]);
//             }
//         	free(arg_copy);
//     	}
//     	i++;
// 	}
// 	return (EXIT_SUCCESS);
// }

static char **create_new_envp(char **envp)
{
    int i = 0;
    char **new_envp;
    int j;

    printf("create_new_envp\n");
    while (envp[i]) {
        i++;
    }
    printf("Number of environment variables: %d\n", i);

    new_envp = malloc(sizeof(char *) * (i + 1));  // Allocate space for i + 1 (NULL terminator)
    if (!new_envp) {
        printf("Memory allocation failed for new_envp\n");
        return NULL;
    }
    printf("create_new_envp2\n");

    for (j = 0; j < i; j++) {
        printf("envp[%d] address: %p, value: %s\n", j, (void *)envp[j], envp[j]);
        new_envp[j] = ft_strdup(envp[j]);
        if (!new_envp[j]) {
            printf("Memory allocation failed for new_envp[%d]\n", j);
            while (--j >= 0) {
                free(new_envp[j]);
            }
            free(new_envp);
            return NULL;
        }
        printf("new_envp[%d] address: %p, value: %s\n", j, (void *)new_envp[j], new_envp[j]);
    }

    new_envp[i] = NULL;  // Null-terminate the new envp array
    printf("create_new_envp4\n");

    return new_envp;
}

static int set_new_var(char **new_envp, char *key, char *value, int index)
{
    char *new_var;

    printf("setnewvar\n");
    if (!key || !value)
    {
        write_error("export", "key or value is NULL\n", NULL);
        return (EXIT_FAILURE);
    }
    new_var = malloc(ft_strlen(key) + ft_strlen(value) + 2);
    if (!new_var)
        return (write_error("export: ", "memory allocation failed\n", NULL));
    ft_strcpy(new_var, key);
    ft_strcat(new_var, "=");
    ft_strcat(new_var, value);
    new_envp[index] = new_var;
    printf("new_envp[%d] address: %p, value: %s\n", index, (void *)new_envp[index], new_envp[index]);
    index++;
    printf("new_envp[%d] address: %p, value: %s\n", index, (void *)new_envp[index], new_envp[index]);
    return (EXIT_SUCCESS);
}

static int set_env_var(char ***envp, const char *key, const char *value)
{
    int i;
    char **new_envp;
    int index;

    printf("set_env_var\n");

    // Create new environment array
    new_envp = create_new_envp(*envp);
    if (!new_envp)
        return (EXIT_FAILURE);

    // Calculate the index of the new variable
    i = 0;
    while ((*envp)[i])
        i++;
    index = i;

    // Attempt to set the new variable
    if (set_new_var(new_envp, (char *)key, (char *)value, index) != EXIT_SUCCESS)
    {
        // Free new_envp if setting the variable failed
        for (int k = 0; k < index; k++)
            free(new_envp[k]);
        free(new_envp);
        return (EXIT_FAILURE);
    }

    // Free the old environment and assign the new one
    if (*envp)
    {
        for (int k = 0; (*envp)[k]; k++)
            free((*envp)[k]);
        free(*envp);
    }

    // Assign the new environment array to *envp
    *envp = new_envp;
    printf("Updated environment:\n");
    for (int j = 0; (*envp)[j] != NULL; j++)
    {
        printf("envp[%d] address: %p, value: %s\n", j, (void *)(*envp)[j], (*envp)[j]);
    }
    return (EXIT_SUCCESS);
}

static void print_sorted_env(char **envp)
{
    int i;
    int j;
    char *tmp;

    i = 0;
    while (envp[i])
    {
        j = i + 1;
        while (envp[j])
        {
            if (envp[i] && envp[j] && ft_strcmp(envp[i], envp[j]) > 0)
            {
                tmp = envp[i];
                envp[i] = envp[j];
                envp[j] = tmp;
            }
            j++;
        }
        i++;
    }
    i = 0;
    while (envp[i])
        write_string("declare -x ", envp[i++], "\n");
    return;
}

int ft_export(int argc, char **argv, char ***envp)
{
    int i;
    char *key;
    char *value;
    char *save_ptr;

    printf("ft_export\n");
    for (int j = 0; (*envp)[j] != NULL; j++)
    {
        printf("envp[%d] address: %p, value: %s\n", j, (void *)(*envp)[j], (*envp)[j]);
    }
    (void)argc;
    i = 1;
    if (!argv[i])
    {
        print_sorted_env(*envp);
        return (EXIT_SUCCESS);
    }
    while (argv[i])
    {
        if (!is_valid_env_var_key(argv[i]))
            return (write_error("export", argv[i], "not a valid identifier\n"));
        else if (ft_strchr(argv[i], '=') != NULL)
        {
            char *arg_copy = ft_strdup(argv[i]);
            if (!arg_copy)
                return (write_error("export", "memory allocation failed\n", NULL));
            key = ft_strtok(arg_copy, "=", &save_ptr);
            value = ft_strtok(NULL, "=", &save_ptr);
            if (set_env_var(envp, key, value) != EXIT_SUCCESS)
            {
                free(arg_copy);
                return (EXIT_FAILURE);
            }
            printf("Updated environment after setting %s=%s:\n", key, value);
            for (int j = 0; (*envp)[j] != NULL; j++)
            {
                printf("envp[%d] address: %p, value: %s\n", j, (void *)(*envp)[j], (*envp)[j]);
            }
            free(arg_copy);
        }
        i++;
    }
    return (EXIT_SUCCESS);
}
