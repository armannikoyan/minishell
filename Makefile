UNAME_S := $(shell uname -s)

# ===============================
# OS Specific Settings (Readline)
# ===============================
#
ifeq ($(UNAME_S),Darwin)
	READLINE_PREFIX := $(shell brew --prefix readline)
	CPPFLAGS += -I$(READLINE_PREFIX)/include
	LDFLAGS += -L$(READLINE_PREFIX)/lib
endif

#
ifeq ($(UNAME_S),Linux)
	LDFLAGS += -lncurses
endif

LDLIBS += -lreadline

# ===============================
# Project Settings
# ===============================
NAME = minishell
SRC_DIR = src
OBJ_DIR = obj
LIBFT_DIR = libs/libft

#  -Wall -Wextra -Werror, includes
CFLAGS := -Wall -Wextra -Werror -I$(LIBFT_DIR) -Iincludes

DEP = includes/minishell.h
LIBFT = $(LIBFT_DIR)/libft.a

# ===============================
# Sources
# ===============================
# Mapped from CMakeLists.txt
SRC = main.c minishell.c \
      $(addprefix get_next_line/, \
          get_next_line.c \
      ) \
      $(addprefix term_settings/, \
          signals.c term_config.c \
      ) \
      $(addprefix utils/, \
          error.c insert_env.c \
      ) \
      $(addprefix hash_table/, \
          hash_table.c hash_table_utils.c \
      ) \
      $(addprefix ast/, \
          ast.c ast_node_creation.c ast_print_module.c \
          ast_print_module_utils.c ast_utils.c ast_syntax_check.c \
          ast_node_deletion.c \
      ) \
      $(addprefix tokenization/, \
          tokenization.c tokenization_utils.c \
          tokenization_node_creation_helper.c tokenization_node_creation.c \
      ) \
      $(addprefix builtin/, \
          pwd.c exit.c env.c echo.c export.c unset.c \
      ) \
      $(addprefix builtin/cd/, \
          cd.c cd_path_utils.c cd_path_parser.c cd_path_resolver.c \
      ) \
      $(addprefix path_utils/, \
          path_utils.c \
      ) \
      $(addprefix execution/, \
          execution.c execution_utils.c execution_redir_node.c \
          execution_binary_node.c execution_command_node.c execution_helper.c \
          redir_utils.c redir_setup_cleanup.c command_env_utils.c \
          command_path_utils.c command_helper.c binary_helper.c \
      ) \
      $(addprefix execution/expansion/, \
          expansion.c expansion_utils.c wildcard.c wildcard_utils.c \
      )

OBJ = $(SRC:%.c=$(OBJ_DIR)/%.o)

CC = cc
RM = rm -f

# ===============================
# Rules
# ===============================
all: $(NAME)

# Compiles objects mirroring the source directory structure
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ) $(LIBFT) $(DEP)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(OBJ) $(LIBFT) $(LDFLAGS) $(LDLIBS) -o $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR) all
	# make -C $(LIBFT_DIR) bonus # Uncomment if your libft has a bonus rule

clean:
	$(RM) -r $(OBJ_DIR)
	make -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME)
	make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re