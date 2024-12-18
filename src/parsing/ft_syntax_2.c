#include "../../includes/minishell.h"

static bool	ft_check_initial_syntax_error(t_list **lst)
{
	char	*err_msg;

	if (*lst && (t_token *)(*lst)->content
		&& (((t_token *)(*lst)->content)->type != 'X')
		&& ((((t_token *)(*lst)->content)->content)[0] != '<'
		|| (((t_token *)(*lst)->content)->content)[0] != '>')
		&& (*lst)->next == NULL)
	{
		err_msg = ft_strjoin("parse error near: ",
				((t_token *)(*lst)->content)->content);
		ft_error(&err_msg, 1);
		return (true);
	}
	return (false);
}

static bool	ft_check_command_not_found(t_token *token)
{
	char	*err_msg;

	if (token->type == 'E')
	{
		err_msg = ft_strjoin("command not found: ", token->content);
		ft_error(&err_msg, 127);
		return (true);
	}
	return (false);
}

static bool	ft_check_invalid_operator_sequence(t_token *token, t_list *tmp)
{
	char	*err_msg;

	if (token->type == 'O' && tmp->next && tmp->next->content
		&& ((t_token *)tmp->next->content)->type == 'O'
		&& ((((t_token *)tmp->next->content)->content)[0] != '<'
		&& (((t_token *)tmp->next->content)->content)[0] != '>'))
	{
		err_msg = ft_strjoin("parse error near: ",
				((t_token *)tmp->next->content)->content);
		ft_error(&err_msg, 1);
		return (true);
	}
	return (false);
}

static bool	ft_check_missing_file(t_token *token, t_list *tmp)
{
	char	*err_msg;

	if (token->type == 'O' && ft_strlen(token->content) == 1
		&& (token->content[0] == '<' || token->content[0] == '>')
		&& tmp->next && ((t_token *)tmp->next->content)->type != 'F')
	{
		err_msg = ft_strjoin("no such file or directory: ",
				((t_token *)tmp->next->content)->content);
		ft_error(&err_msg, 1);
		return (true);
	}
	return (false);
}
