CFLAGS := -Wall -Wextra -Werror -I$(LIBFT_DIR) ${CPPFLAGS}
LDFLAGS := ${LDFLAGS}
LIB = -lreadline

NAME = minishell
SRC_DIR = src
OBJ_DIR = obj
LIBFT_DIR = libs/libft
DEP = includes/minishell.h

LIBFT = $(LIBFT_DIR)/libft.a

SRC = main.c minishell.c \
      $(addprefix utils/, \
      error.c signals.c term_config.c \
      dublicate_shell_env.c cursor.c \
      ) \

OBJ = $(SRC:%.c=$(OBJ_DIR)/%.o)

CC = cc
RM = rm -f

all: $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ) $(LIBFT) $(DEP)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(LDFLAGS) $(LIB) -o $(NAME)

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
