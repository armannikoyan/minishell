#ifndef TERM_SETTINGS_H
# define TERM_SETTINGS_H

# include <termios.h>

// --------- TERMINAL SETTINGS ---------
void set_term_config(struct termios *);

void restore_terminal_settings(const struct termios *);

// --------- SIGNAL HANDLERS ---------
void psig_set(void);

void psig_handler(int);

#endif
