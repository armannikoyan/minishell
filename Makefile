NAME = minishell

SRC_DIR = src
OBJ_DIR = obj
LIBFT_DIR = libs/libft

DEPS = includes/minishell.h

SRCS = main.c

# OBJ_SUBDIRS = $(addprefix $(OBJ_DIR)/, \
# 		utils)

OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

CC = cc
CFLAGS = -Wall -Wextra -Werror -Ireadline -Ilibs/libft
INC = -Ireadline -Ilibs/libft
LIBFT = $(LIBFT_DIR)/libft.a

RM = rm -f

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
	# mkdir -p $(OBJ_SUBDIRS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(NAME): $(OBJS) $(LIBFT) $(DEPS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR) all
	make -C $(LIBFT_DIR) bonus

all: $(NAME)

bonus: all

clean:
	$(RM) -r $(OBJ_DIR)
	make -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME)
	make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all bonus fclean clean
