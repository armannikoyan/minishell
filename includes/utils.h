#ifndef UTILS_H
# define UTILS_H

# include <termios.h>
#include "ast.h"

// --------- TERMINAL SETTINGS ---------
void	set_term_config(struct termios *original);
void	restore_terminal_settings(struct termios *original);

// --------- SIGNAL HANDLERS ---------
void	psig_set(void);
void	psig_handler(int signo);

// --------- HELPER FUNCTIONS ---------
void	move_cursor_up(void);

// --------- ERROR PRINTING ---------
void	print_error(const char *err);

// Prints abstract tree
void	print_ast(t_ast_node *root);

#endif
