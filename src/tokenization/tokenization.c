#include <stddef.h>
#include <stdlib.h>

#include "tokenization.h"
#include "utils.h"
#include "../../libs/libft/libft.h"

t_ast_node	*tokenize(char *input)
{
	t_ast_node	*node;
	t_node_type	type;
	size_t	i;
	size_t	op_len;
	char	*sub_str;
	t_ast_node *sub_tree;
    t_ast_node *head_node;

	i = 0;
	node = NULL;
    head_node = NULL;
	while (input[i])
	{
		while (input[i] == ' ')
			++i;
		if (!input[i])
			break ;
		type = get_node_type(&(input[i]));
		if (type == SUBSHELL_NODE)
		{
			sub_str = extract_subshell_content(input, &i);
			if (!sub_str)
				return (NULL);
			if (ft_strlen(sub_str) == 0) {
				free(sub_str);
				continue;
			}
			sub_tree = tokenize(sub_str);
			free(sub_str);
			if (!sub_tree)
				return (NULL);
			node = create_subshell_node(SUBSHELL_NODE, sub_tree);
		}
		else if (type == ERROR_NODE)
		{
			// TODO: Change to fd 2
			//		 Change the content of the message
			ft_printf("Minishell: syntax error near unexpected token `%c'\n", input[i]);
			return (NULL);
		}
		else if (type == COMMAND_NODE)
			node = create_cmd_node(COMMAND_NODE, get_argv(input, &i));
		else if (is_operator(&(input[i])) || is_redir(&(input[i])))
		{
			op_len = get_operator_len(type);
			if (type >= REDIRECT_IN_NODE && type <= HEREDOC_NODE)
			{
				i += op_len;
				node = create_redir_node(type, substr_next(input, &i));
			}
			else
			{
				i += op_len;
				node = create_binary_node(type);
			}
		}
		if (node) {
			head_node = ast_build(node, head_node);
			print_ast_info(head_node, node);
		}
	}
	return (node);
}
