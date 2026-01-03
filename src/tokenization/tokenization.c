#include <stddef.h>
#include <stdlib.h>

#include "../../includes/tokenization.h"
#include "../../libs/libft/libft.h"

static char *substr_next(char *input, size_t *i) {
    char *tmp;
    size_t len;
    size_t start;
    size_t j;

    while (input[*i] == ' ')
        ++(*i);
    start = *i;
    while (input[*i] && input[*i] != ' '
           && !is_operator(&(input[*i])) && !is_redir(&(input[*i])))
        ++(*i);
    len = *i - start;
    tmp = (char *) malloc(sizeof(char) * (len + 1));
    if (!tmp)
        return (NULL);
    j = 0;
    while (j < len) {
        tmp[j] = input[start + j];
        ++j;
    }
    tmp[j] = '\0';
    return (tmp);
}

static char **get_argv(char *input, size_t *i) {
    char **argv;
    size_t len;
    size_t j;

    j = *i;
    len = 0;
    while (input[j] && !is_operator(&(input[j])) && !is_redir(&(input[j]))) {
        while (input[j] == ' ')
            ++j;
        if (input[j] && !is_operator(&(input[j])) && !is_redir(&(input[j]))) {
            ++len;
            while (input[j] && input[j] != ' '
                   && !is_operator(&(input[j])) && !is_redir(&(input[j])))
                ++j;
        }
    }
    if (len == 0)
        return (NULL);
    argv = (char **) malloc(sizeof(char *) * (len + 1));
    if (!argv)
        return (NULL);
    j = 0;
    while (j < len)
        argv[j++] = substr_next(input, i);
    argv[len] = NULL;
    return (argv);
}

t_node_type get_node_type(char *input) {
    if (!ft_strncmp(input, "<<", 2))
        return (NODE_HEREDOC);
    else if (*input == '<')
        return (NODE_REDIRECT_IN);
    else if (!ft_strncmp(input, ">>", 2))
        return (NODE_REDIRECT_APPEND);
    else if (*input == '>')
        return (NODE_REDIRECT_OUT);
    else if (!ft_strncmp(input, "||", 2))
        return (NODE_OR);
    else if (*input == '|')
        return (NODE_PIPE);
    else if (!ft_strncmp(input, "&&", 2))
        return (NODE_AND);
    return (NODE_COMMAND);
}

size_t get_operator_len(t_node_type type) {
    if (type == NODE_HEREDOC || type == NODE_REDIRECT_APPEND
        || type == NODE_OR || type == NODE_AND)
        return (2);
    else if (type == NODE_REDIRECT_IN || type == NODE_REDIRECT_OUT
             || type == NODE_PIPE)
        return (1);
    return (0);
}

t_ast_node *tokenize(char *input) {
    t_ast_node *node;
    t_ast_node *head_node;
    t_node_type type;
    size_t i;
    size_t op_len;

    i = 0;
    node = NULL;
    head_node = NULL;


    while (input[i]) {
        while (input[i] == ' ')
            ++i;
        if (!input[i])
            break ;
        type = get_node_type(&(input[i]));
        if (type == NODE_COMMAND)
            node = create_cmd_node(NODE_COMMAND, get_argv(input, &i));
        else if (is_operator(&(input[i])) || is_redir(&(input[i]))) {
            op_len = get_operator_len(type);
            if (type >= NODE_REDIRECT_IN && type <= NODE_HEREDOC) {
                i += op_len;
                node = create_redir_node(type, substr_next(input, &i), -1);
            } else {
                i += op_len;
                node = create_binary_node(type);
            }
        }
        if (node) {
            node = ast_build(node, head_node);
            head_node = node;
        }
    }
    return (node);
}
