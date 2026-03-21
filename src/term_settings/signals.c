#include <readline/readline.h>
#include <signal.h>
#include <unistd.h>

void psig_handler(const int signum) {
  if (signum == SIGINT) {
    write(STDOUT_FILENO, "\n", 1);
    rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay();
  }
}

void psig_set(void) {
  struct sigaction sa;

  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sa.sa_handler = psig_handler;
  sigaction(SIGINT, &sa, NULL);
  sa.sa_handler = SIG_IGN;
  sigaction(SIGQUIT, &sa, NULL);
}
