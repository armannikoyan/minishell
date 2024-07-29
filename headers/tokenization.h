/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasargsy <dasargsy@student.42yerevan.am    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 23:12:54 by dasargsy          #+#    #+#             */
/*   Updated: 2024/07/29 23:30:13 by dasargsy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZATION_H
# define TOKENIZATION_H

#include "../../minishell.h"

void    quote_error(char *line);
int     is_spcs(char c);
char    **tokenization(char *line);
int		is_closed(char *line, char limiter);

#endif