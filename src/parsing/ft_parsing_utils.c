#include "../../includes/minishell.h"

bool	has_single_token_error(t_list **lst)
{
	t_token	*token;

	if (!*lst || !(*lst)->content)
		return (true);
	token = (t_token *)(*lst)->content;
	if (*lst && token && token->type != 'X'
		&& (token->content[0] != '<'
			|| token->content[0] != '>')
		&& (*lst)->next == NULL)
		return (ft_report_error("parse error near: ", token->content, 1));
	return (false);
}

void	append_character(char *expanded_input, char c, unsigned short *j)
{
	expanded_input[*j] = c;
	(*j)++;
}
