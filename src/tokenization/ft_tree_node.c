#include "../../includes/minishell.h"

static unsigned short	ft_contentlen(t_list **lst)
{
	t_list	*tmp;

	tmp = *lst;
	if (((t_token *)tmp->content)->type == 'X')
		return (handle_x_type(&tmp));
	else if (((t_token *)tmp->content)->type == 'O')
		return (handle_o_type(tmp));
	return (0);
}

static char	**initialize_content_array(t_list **tmp)
{
	unsigned short	len;
	char			**content;

	len = ft_contentlen(tmp);
	content = (char **)malloc(sizeof(char *) * (len + 1));
	if (!content)
		exit(EXIT_FAILURE);
	return (content);
}

static unsigned short	populate_x_node_content(char **content, t_list **tmp)
{
	unsigned short	i;

	i = 0;
	while (*tmp && ((t_token *)(*tmp)->content)->type != 'O')
	{
		content[i++] = ((t_token *)(*tmp)->content)->content;
		*tmp = (*tmp)->next;
	}
	while (*tmp && (((t_token *)(*tmp)->content)->type == 'O'
		|| ((t_token *)(*tmp)->content)->type == 'F'))
		*tmp = (*tmp)->next;
	while (*tmp && ((t_token *)(*tmp)->content)->type == 'A')
	{
		content[i++] = ((t_token *)(*tmp)->content)->content;
		*tmp = (*tmp)->next;
	}
	return (i);
}

static unsigned short	populate_o_node_content(char **content, t_list **tmp)
{
	unsigned short	i;

	i = 0;
	while (*tmp && (((t_token *)(*tmp)->content)->type == 'O'
		|| ((t_token *)(*tmp)->content)->type == 'F'))
	{
		content[i++] = ((t_token *)(*tmp)->content)->content;
		*tmp = (*tmp)->next;
	}
	return (i);
}

static t_node	*handle_type_node(char **content, t_list **tmp, char type)
{
	unsigned short	i;

	if (type == 'X')
		i = populate_x_node_content(content, tmp);
	else
		i = populate_o_node_content(content, tmp);
	content[i] = NULL;
	return (ft_node_ctor(content, type));
}
