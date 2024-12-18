#include "../../includes/minishell.h"

void	set_command_type(t_token *token)
{
	struct stat	statbuf;

	if (stat(token->content, &statbuf) == 0 && !S_ISDIR(statbuf.st_mode)
		&& (statbuf.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)))
		token->type = 'X';
	else
		token->type = 'E';
}

char	*join_paths(const char *base, const char *suffix)
{
	char	*tmp;
	char	*result;

	tmp = ft_strjoin("/", suffix);
	if (!tmp)
		exit(EXIT_FAILURE);
	result = ft_strjoin(base, tmp);
	free(tmp);
	if (!result)
		exit(EXIT_FAILURE);
	return (result);
}

static bool	ft_identify_command(t_token **token, char **path)
{
	char	*line;
	int		i;
	bool	result;

	i = 0;
	result = (access((*token)->content, X_OK) == 0);
	while (path[i] && !result)
	{
		line = join_paths(path[i], (*token)->content);
		if (access(line, X_OK) == 0)
		{
			free((*token)->content);
			(*token)->content = ft_strdup(line);
			result = true;
		}
		free(line);
		i++;
	}
	ft_free_2d_array((void ***)&path);
	return (result);
}

unsigned short	calculate_token_length(const char *input, unsigned short *index)
{
	unsigned short	len;
	unsigned short	i;

	len = 0;
	i = 0;
	while (input[*index + i] && input[*index + i] != ' ')
	{
		if (input[*index + i] == '\"' || input[*index + i] == '\'')
		{
			i++;
			while (input[*index + i]
				&& input[*index + i] != '\"'
				&& input[*index + i] != '\'')
			{
				i++;
				len++;
			}
		}
		i++;
		len++;
	}
	return (len);
}
