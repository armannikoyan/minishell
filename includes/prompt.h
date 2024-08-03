/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasargsy <dasargsy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 00:04:31 by dasargsy          #+#    #+#             */
/*   Updated: 2024/08/03 20:35:33 by dasargsy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROMPT_H
# define PROMPT_H

# include <signal.h>
# include "../libs/libft/libft.h"

# define GREEN "\x1b[32m"
# define RESET_COLOR "\x1b[0m"
# define UNCLOSED_Q ""

void	prompt(char **envp);
int		check_for_quotes(char *line, int limiter);
int		check_for_parenthesis(char *line);

#endif