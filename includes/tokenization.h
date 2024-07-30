/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasargsy <dasargsy@student.42yerevan.am    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 23:12:54 by dasargsy          #+#    #+#             */
/*   Updated: 2024/07/30 04:23:56 by dasargsy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZATION_H
# define TOKENIZATION_H

# include "../libs/libft/libft.h"

# define QUOTE_ERROR "Unclosed Quotes\n"

void    quote_error(char *line);
char    **tokenization(char *line);
int		is_closed(char *line, char limiter);
int		check_for_backticks(char *line);
int		token_size(char *line, int i);

#endif
