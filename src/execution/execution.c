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

static bool push_frame(t_exec_stack *stack, t_ast_node *node) {
  if (!node)
    return true;

  if (stack->count >= stack->capacity) {
    const size_t new_cap = stack->capacity == 0 ? 64 : stack->capacity * 2;
    t_exec_frame *new_frames =
        realloc(stack->frames, sizeof(t_exec_frame) * new_cap);
    if (!new_frames)
      return false;

    stack->frames = new_frames;
    stack->capacity = new_cap;
  }

  t_exec_frame *new_frame = &stack->frames[stack->count];
  new_frame->node = node;
  new_frame->state = STATE_VISIT_NODE;
  new_frame->saved_fd = -1;
  new_frame->target_fd = -1;

  stack->count++;
  return true;
}

int execute_subshell(const t_ast_node *node, t_hash_table *ht,
                     const int errnum) {
  const pid_t pid = fork();
  if (pid == -1) {
    print_error("minishell: fork", false);
    return 1;
  }

  if (pid) {
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    const int status = handle_child_exit(pid);
    psig_set();
    return status;
  }

  exit(execute(node->u_data.subshell.root, ht, errnum));
}

int execute(t_ast_node *root, t_hash_table *ht, const int errnum) {
  if (!root)
    return 0;

  t_exec_stack stack = {NULL, 0, 0};
  if (!push_frame(&stack, root))
    return 1;

  int exit_status = errnum;

  while (stack.count > 0) {
    t_exec_frame *top_frame = &stack.frames[stack.count - 1];

    if (top_frame->state == STATE_VISIT_NODE) {

      if (top_frame->node->type == PIPE_NODE) {
        exit_status = execute_pipeline(top_frame->node, ht, exit_status);
        stack.count--;
      } else if (top_frame->node->abstract_type == BIN_NODE) {
        top_frame->state = STATE_EVALUATE_RIGHT;

        t_ast_node *left_node = top_frame->node->u_data.binary.left;
        if (!push_frame(&stack, left_node))
          goto unwind_error;
      } else if (top_frame->node->abstract_type == REDIR_NODE) {
        if (setup_redirection(top_frame->node, ht, &top_frame->saved_fd,
                              &top_frame->target_fd, exit_status) != 0) {
          exit_status = 1;
          stack.count--;
        } else {
          top_frame->state = STATE_CLEANUP_REDIR;

          t_ast_node *redir_node = top_frame->node;
          int s_fd = top_frame->saved_fd;
          int t_fd = top_frame->target_fd;
          t_ast_node *child_node = top_frame->node->u_data.redir.child;

          if (!push_frame(&stack, child_node)) {
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
        stack.count--;
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
        stack.count--;
      }
    } else if (top_frame->state == STATE_CLEANUP_REDIR) {
      cleanup_redirection(top_frame->node, top_frame->saved_fd,
                          top_frame->target_fd);
      stack.count--;
    }
  }

  free(stack.frames);
  return exit_status;

unwind_error:
  exit_status = 1;
  while (stack.count > 0) {
    stack.count--;
    const t_exec_frame *frame = &stack.frames[stack.count];
    if (frame->state == STATE_CLEANUP_REDIR) {
      cleanup_redirection(frame->node, frame->saved_fd, frame->target_fd);
    }
  }
  free(stack.frames);
  return exit_status;
}