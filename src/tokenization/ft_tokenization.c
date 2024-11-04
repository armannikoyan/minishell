/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenization.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 20:33:00 by anikoyan          #+#    #+#             */
/*   Updated: 2024/10/22 21:49:53 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*ft_find_token(char *input)
{
	char			*value;
	unsigned short	len;
	unsigned short	i;

	i = 0;
	len = 0;
	while (input[i] && input[i] != ' ')
	{
		len++;
		i++;
	}
	value = (char *)malloc(sizeof(char) * (len + 1));
	i = 0;
	while (input[i] && input[i] != ' ')
	{
		value[i] = input[i];
		i++;
	}
	value[i] = '\0';
	return (value);
}

t_list	**ft_tokenization(char *input)
{
	t_list	**lst;
	t_list	*tmp;
	char	*value;
	unsigned int	i;

	value = NULL;
	i = 0;
	lst = (t_list **)malloc(sizeof(t_list *));
	if (!lst)
		exit(-1);
	*lst = NULL;
	while (input[i])
	{
		value = ft_find_token(&input[i]);
		if (!value)
			exit(-1);
		tmp = (t_list *)malloc(sizeof(t_list));
		tmp->content = value;
		tmp->next = NULL;
		ft_lstadd_back(lst, tmp);
		i += ft_strlen(value) + 1;
	}
	return (lst);
}
