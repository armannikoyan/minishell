/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasargsy <dasargsy@student.42yerevan.am    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 00:04:31 by dasargsy          #+#    #+#             */
/*   Updated: 2024/07/30 02:27:44 by dasargsy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROMPT_H
# define PROMPT_H

# include <signal.h>
# include "../libs/libft/libft.h"

# define GREEN "\x1b[32m"
# define RESET_COLOR "\x1b[0m"

void	prompt(char **argv);

#endif