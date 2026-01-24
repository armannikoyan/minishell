#include <stddef.h>

#include "ast.h"
#include "utils.h"
#include "error_codes.h"
#include <sys/errno.h>

#include "../../libs/libft/libft.h"

int syntax_check(t_ast_node *node, int *errnum)
{
    if (!node)
        return (0);
    if (node->abstract_type == BIN_NODE)
    {
        if (node->u_data.binary.left == NULL || node->u_data.binary.right == NULL)
        {
            print_syntax_error(node, errnum);
            *errnum = SYNTAX_ERROR;
            return (SYNTAX_ERROR);
        }
        if (syntax_check(node->u_data.binary.left, errnum) != 0)
            return (SYNTAX_ERROR);
        if (syntax_check(node->u_data.binary.right, errnum) != 0)
            return (SYNTAX_ERROR);
    }
    else if (node->abstract_type == REDIR_NODE)
    {
        if (ft_strcmp(node->u_data.redir.filename, "") == 0)
        {
            print_error("minishell: syntax error near token `", true);
            print_error(get_type(node->type), true);
            if (node->type == HEREDOC_NODE)
                print_error("\': No EOL presented\n", true);
            else
                print_error("\': No filename presented\n", true);
            *errnum = SYNTAX_ERROR;
            return (SYNTAX_ERROR);
        }
        if (node->u_data.redir.child)
        {
            if (syntax_check(node->u_data.redir.child, errnum) != 0)
                return (SYNTAX_ERROR);
        }
    }
    else if (node->type == SUBSHELL_NODE)
    {
        if (node->u_data.subshell.root == NULL)
        {
            print_syntax_error(node, errnum);
            *errnum = SYNTAX_ERROR;
            return (SYNTAX_ERROR);
        }
        if (syntax_check(node->u_data.subshell.root, errnum) != 0)
            return (SYNTAX_ERROR);
    }
    return (0);
}
