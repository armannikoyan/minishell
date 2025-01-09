#include "../../includes/minishell.h"

bool	ft_isbuiltin(t_token *token)
{
	if (ft_strcmp(token->content, "echo") == 0
		|| ft_strcmp(token->content, "cd") == 0
		|| ft_strcmp(token->content, "pwd") == 0
		|| ft_strcmp(token->content, "export") == 0
		|| ft_strcmp(token->content, "unset") == 0
		|| ft_strcmp(token->content, "env") == 0
		|| ft_strcmp(token->content, "exit") == 0)
		return (true);
	return (false);
}

void	ft_handle_argument(t_list **tmp, t_token *token)
{
	token->type = 'X';
	ft_assign_argument_type(tmp);
}

void	ft_assign_subshell_levels(t_list **lst)
{
	t_list	*tmp;
	t_list	*prev;
	int		current_level;

	tmp = *lst;
	prev = NULL;
	current_level = 0;
	while (tmp)
		ft_handle_token(lst, &tmp, &prev, &current_level);
}

void	ft_remove_subshell_token(t_list **lst, t_list **tmp, t_list *prev)
{
	if (prev)
		prev->next = (*tmp)->next;
	else
		*lst = (*tmp)->next;
	ft_lstdelone(*tmp, ft_tokendelone);
	if (prev)
		*tmp = prev->next;
	else
		*tmp = *lst;
}

void	ft_handle_token(t_list **lst,
		t_list **tmp, t_list **prev, int *current_level)
{
	t_token	*token;

	token = (t_token *)(*tmp)->content;
	if (ft_strcmp(token->content, "(") == 0)
	{
		(*current_level)++;
		ft_remove_subshell_token(lst, tmp, *prev);
	}
	else if (ft_strcmp(token->content, ")") == 0)
	{
		if (*current_level > 0)
			(*current_level)--;
		ft_remove_subshell_token(lst, tmp, *prev);
	}
	else
	{
		token->subshell_level = *current_level;
		*prev = *tmp;
		*tmp = (*tmp)->next;
	}
}
