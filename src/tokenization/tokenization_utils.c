/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 18:57:24 by anikoyan          #+#    #+#             */
/*   Updated: 2025/12/12 18:58:03 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>

#include "../../libs/libft/libft.h"

bool	is_redir(char *str)
{
	if (!ft_strncmp(str, "<<", 2) || *str == '<'
		|| !ft_strncmp(str, ">>", 2) || *str == '>')
		return (true);
	return (false);
}

bool	is_operator(char *str)
{
	if (!ft_strncmp(str, "||", 2) || *str == '|'
		|| !ft_strncmp(str, "&&", 2))
		return (true);
	return (false);
}
