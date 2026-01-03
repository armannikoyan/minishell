CFLAGS := -Wall -Wextra -Werror -I$(LIBFT_DIR) -Iincludes

ifeq ($(UNAME_S),Darwin)
	READLINE_PREFIX := $(shell brew --prefix readline)
	CPPFLAGS += -I$(READLINE_PREFIX)/include
	LDFLAGS  += -L$(READLINE_PREFIX)/lib
endif
LDFLAGS += -lreadline

NAME = minishell
SRC_DIR = src
OBJ_DIR = obj
LIBFT_DIR = libs/libft
DEP = includes/minishell.h

LIBFT = $(LIBFT_DIR)/libft.a

SRC = main.c minishell.c \
      $(addprefix utils/, \
      error.c signals.c term_config.c \
      cursor.c \
      ) \
      $(addprefix hash_table/, \
      hash_table.c hash_table_utils.c \
      ) \
      $(addprefix ast/, \
      ast.c ast_utils.c \
      ) \
      $(addprefix tokenization/, \
      tokenization.c tokenization_utils.c\
      ) \

OBJ = $(SRC:%.c=$(OBJ_DIR)/%.o)

CC = cc
RM = rm -f

all: $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ) $(LIBFT) $(DEP)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(OBJ) $(LIBFT) -o $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR) all
	make -C $(LIBFT_DIR) bonus

clean:
	$(RM) -r $(OBJ_DIR)
	make -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME)
	make -C $(LIBFT_DIR) fclean

re: fclean all

bonus: all

.PHONY: all bonus clean fclean re
