#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <readline/readline.h>

#include "ast.h"
#include "execution.h"
#include "expansion.h"
#include "hash_table.h"
#include "tokenization.h"
#include "utils.h"

static void generate_unique_filename(char *buffer, const size_t buf_size,
                                     const int index) {
  snprintf(buffer, buf_size, "/tmp/.heredoc_%d_%d", getpid(), index);
}

static int setup_heredoc(t_ast_node *node, t_hash_table *ht, int *saved_fd,
                         const int errnum) {
  char filename[PATH_MAX];
  generate_unique_filename(filename, sizeof(filename), 9999);

  *saved_fd = dup(STDIN_FILENO);
  if (*saved_fd == -1) {
    print_error("minishell: dup", false);
    return 1;
  }

  int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd == -1) {
    close(*saved_fd);
    print_error("minishell: open", false);
    return 1;
  }

  char quote_char = 0;
  for (size_t i = 0; node->u_data.redir.filename[i]; i++) {
    set_quote_char(node->u_data.redir.filename[i], &quote_char);
    if (quote_char)
      break;
  }

  char *tmp_name = node->u_data.redir.filename;
  node->u_data.redir.filename = remove_quotes(tmp_name);
  free(tmp_name);

  if (!node->u_data.redir.filename) {
    close(fd);
    close(*saved_fd);
    print_error("minishell: malloc", false);
    return 1;
  }

  while (true) {
    char *line = readline("> ");
    if (!line || strcmp(line, node->u_data.redir.filename) == 0) {
      if (!line) {
        print_error("minishell: warning: here-document delimited by "
                    "end-of-file (wanted `",
                    true);
        print_error(node->u_data.redir.filename, true);
        print_error("')\n", true);
      }
      free(line);
      break;
    }

    if (!quote_char) {
      tmp_name = line;
      line = expand_dollar_sign(tmp_name, ht, errnum);
      free(tmp_name);
    }

    if (line) {
      write(fd, line, strlen(line));
      write(fd, "\n", 1);
      free(line);
    }
  }
  close(fd);

  fd = open(filename, O_RDONLY);
  if (fd == -1) {
    close(*saved_fd);
    print_error("minishell: open", false);
    return 1;
  }
  if (dup2(fd, STDIN_FILENO) == -1) {
    close(fd);
    close(*saved_fd);
    print_error("minishell: dup2", false);
    return 1;
  }

  close(fd);
  unlink(filename);
  return 0;
}

static int setup_file_redir(t_ast_node *node, const int open_flags,
                            const int std_fd, int *saved_fd) {
  *saved_fd = dup(std_fd);
  if (*saved_fd == -1) {
    print_error("minishell: dup", false);
    return 1;
  }

  char *tmp = node->u_data.redir.filename;
  node->u_data.redir.filename = remove_quotes(tmp);
  free(tmp);

  if (!node->u_data.redir.filename) {
    close(*saved_fd);
    print_error("minishell: malloc", false);
    return 1;
  }

  node->u_data.redir.fd = open(node->u_data.redir.filename, open_flags, 0644);
  if (node->u_data.redir.fd == -1) {
    print_error("minishell: open", false);
    close(*saved_fd);
    return 1;
  }

  if (dup2(node->u_data.redir.fd, std_fd) == -1) {
    print_error("minishell: dup2", false);
    close(node->u_data.redir.fd);
    close(*saved_fd);
    return 1;
  }

  close(node->u_data.redir.fd);
  return 0;
}

int setup_redirection(t_ast_node *node, t_hash_table *ht, int *saved_fd,
                      int *target_fd, const int errnum) {
  if (node->type == REDIRECT_IN_NODE) {
    *target_fd = STDIN_FILENO;
    return setup_file_redir(node, O_RDONLY, STDIN_FILENO, saved_fd);
  }
  if (node->type == REDIRECT_OUT_NODE) {
    *target_fd = STDOUT_FILENO;
    return setup_file_redir(node, O_WRONLY | O_CREAT | O_TRUNC, STDOUT_FILENO,
                            saved_fd);
  }
  if (node->type == REDIRECT_APPEND_NODE) {
    *target_fd = STDOUT_FILENO;
    return setup_file_redir(node, O_WRONLY | O_CREAT | O_APPEND, STDOUT_FILENO,
                            saved_fd);
  }
  if (node->type == HEREDOC_NODE) {
    *target_fd = STDIN_FILENO;
    return setup_heredoc(node, ht, saved_fd, errnum);
  }
  return 1;
}

int cleanup_redirection(t_ast_node *node __attribute((unused)),
                        const int saved_fd, const int target_fd) {
  if (dup2(saved_fd, target_fd) == -1) {
    print_error("minishell: dup2", false);
    close(saved_fd);
    return 1;
  }

  close(saved_fd);
  return 0;
}

int scan_and_process_heredoc(t_ast_node *node, t_hash_table *ht, int *counter) {
  if (!node)
    return 0;

  if (node->type == HEREDOC_NODE) {
    char filename[PATH_MAX];
    generate_unique_filename(filename, sizeof(filename), (*counter)++);

    const int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
      print_error("minishell: open heredoc", false);
      return 1;
    }

    char *limiter = remove_quotes(node->u_data.redir.filename);

    while (true) {
      char *line = readline("> ");
      if (!line)
        break;

      if (strcmp(line, limiter) == 0) {
        free(line);
        break;
      }

      write(fd, line, strlen(line));
      write(fd, "\n", 1);
      free(line);
    }

    close(fd);
    free(limiter);

    free(node->u_data.redir.filename);
    node->u_data.redir.filename = strdup(filename);
    node->type = REDIRECT_IN_NODE;
  }

  if (node->type == PIPE_NODE || node->abstract_type == BIN_NODE) {
    if (scan_and_process_heredoc(node->u_data.binary.left, ht, counter))
      return 1;
    if (scan_and_process_heredoc(node->u_data.binary.right, ht, counter))
      return 1;
  } else if (node->abstract_type == REDIR_NODE) {
    if (scan_and_process_heredoc(node->u_data.redir.child, ht, counter))
      return 1;
  }

  return 0;
}

void cleanup_heredoc_files(const int count) {
  char filename[PATH_MAX];
  for (int i = 0; i < count; i++) {
    generate_unique_filename(filename, sizeof(filename), i);
    unlink(filename);
  }
}