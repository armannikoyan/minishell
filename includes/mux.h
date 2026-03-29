#ifndef MUX_H
#define MUX_H

#include <stdio.h>
#include <sys/un.h>

#define MUX_SOCKET_PREFIX "/tmp/minishell_mux_"
#define MUX_SOCKET_EXT ".sock"

#define FORMAT_SOCKET_PATH(buf, size, session_name)                            \
  snprintf((buf), (size), "%s%s%s", MUX_SOCKET_PREFIX, (session_name),         \
           MUX_SOCKET_EXT)

int run_daemon(char **envp, const char *session_name);
int run_client(const char *session_name);
int auto_attach_or_spawn(char **envp, const char *session_name);

#endif