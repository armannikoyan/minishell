#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#include <signal.h>

#include "ast.h"
#include "execution.h"
#include "hash_table.h"
#include "term_settings.h"
#include "utils.h"

typedef enum {
  STATE_VISIT_NODE = 0,
  STATE_EVALUATE_RIGHT,
  STATE_CLEANUP_REDIR
} t_exec_state;

typedef struct {
  t_ast_node *node;
  t_exec_state state;
  int saved_fd;
  int target_fd;
} t_exec_frame;

typedef struct {
  t_exec_frame *frames;
  size_t count;
  size_t capacity;
} t_exec_stack;

static t_exec_stack g_stack = {NULL, 0, 0};

extern void free_resources(int code);

void cleanup_exec_stack(void) {
  if (g_stack.frames) {
    free(g_stack.frames);
    g_stack.frames = NULL;
    g_stack.count = 0;
    g_stack.capacity = 0;
  }
}

static bool push_frame(t_ast_node *node) {
  if (!node)
    return true;

  if (g_stack.count >= g_stack.capacity) {
    const size_t new_cap = g_stack.capacity == 0 ? 64 : g_stack.capacity * 2;
    t_exec_frame *new_frames =
        realloc(g_stack.frames, sizeof(t_exec_frame) * new_cap);
    if (!new_frames)
      return false;

    g_stack.frames = new_frames;
    g_stack.capacity = new_cap;
  }

  t_exec_frame *new_frame = &g_stack.frames[g_stack.count];
  new_frame->node = node;
  new_frame->state = STATE_VISIT_NODE;
  new_frame->saved_fd = -1;
  new_frame->target_fd = -1;

  g_stack.count++;
  return true;
}

void close_saved_fds(void) {
  for (size_t i = 0; i < g_stack.count; i++) {
    if (g_stack.frames[i].state == STATE_CLEANUP_REDIR &&
        g_stack.frames[i].saved_fd != -1) {
      close(g_stack.frames[i].saved_fd);
      g_stack.frames[i].saved_fd = -1;
    }
  }
}

int execute_subshell(const t_ast_node *node, t_hash_table *ht,
                     const int errnum) {
  const pid_t pid = fork();
  if (pid == -1) {
    print_error("minishell: fork", false);
    return 1;
  }

  if (pid == 0) {
    close_saved_fds();
    const int code = execute(node->u_data.subshell.root, ht, errnum);
    free_resources(code);
  } else {
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    const int status = handle_child_exit(pid);
    psig_set();
    return status;
  }

  return 1;
}

int execute(t_ast_node *root, t_hash_table *ht, const int errnum) {
  if (!root)
    return 0;

  g_stack.count = 0;
  if (!push_frame(root))
    return 1;

  int exit_status = errnum;

  while (g_stack.count > 0) {
    t_exec_frame *top_frame = &g_stack.frames[g_stack.count - 1];

    if (top_frame->state == STATE_VISIT_NODE) {

      if (top_frame->node->type == PIPE_NODE) {
        exit_status = execute_pipeline(top_frame->node, ht, exit_status);
        g_stack.count--;
      } else if (top_frame->node->abstract_type == BIN_NODE) {
        top_frame->state = STATE_EVALUATE_RIGHT;

        t_ast_node *left_node = top_frame->node->u_data.binary.left;
        if (!push_frame(left_node))
          goto unwind_error;
      } else if (top_frame->node->abstract_type == REDIR_NODE) {
        if (setup_redirection(top_frame->node, ht, &top_frame->saved_fd,
                              &top_frame->target_fd, exit_status) != 0) {
          exit_status = 1;
          g_stack.count--;
        } else {
          top_frame->state = STATE_CLEANUP_REDIR;

          t_ast_node *redir_node = top_frame->node;
          const int s_fd = top_frame->saved_fd;
          const int t_fd = top_frame->target_fd;
          t_ast_node *child_node = top_frame->node->u_data.redir.child;

          if (!push_frame(child_node)) {
            cleanup_redirection(redir_node, s_fd, t_fd);
            goto unwind_error;
          }
        }
      } else {
        if (top_frame->node->type == SUBSHELL_NODE) {
          exit_status = execute_subshell(top_frame->node, ht, exit_status);
        } else if (top_frame->node->abstract_type == CMD_NODE) {
          exit_status = execute_command(top_frame->node, ht, exit_status);
        }
        g_stack.count--;
      }

    } else if (top_frame->state == STATE_EVALUATE_RIGHT) {
      bool perform_right = false;
      if (top_frame->node->type == AND_NODE && exit_status == 0)
        perform_right = true; // NOLINT(*-branch-clone)
      else if (top_frame->node->type == OR_NODE && exit_status != 0)
        perform_right = true;

      if (perform_right) {
        top_frame->node = top_frame->node->u_data.binary.right;
        top_frame->state = STATE_VISIT_NODE;
      } else {
        g_stack.count--;
      }
    } else if (top_frame->state == STATE_CLEANUP_REDIR) {
      cleanup_redirection(top_frame->node, top_frame->saved_fd,
                          top_frame->target_fd);
      g_stack.count--;
    }
  }

  cleanup_exec_stack();
  return exit_status;

unwind_error:
  exit_status = 1;
  while (g_stack.count > 0) {
    g_stack.count--;
    const t_exec_frame *frame = &g_stack.frames[g_stack.count];
    if (frame->state == STATE_CLEANUP_REDIR) {
      cleanup_redirection(frame->node, frame->saved_fd, frame->target_fd);
    }
  }
  cleanup_exec_stack();
  return exit_status;
}