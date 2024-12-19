NAME = minishell

SRC_DIR = src
OBJ_DIR = obj
LIBFT_DIR = libs/libft

DEPS = includes/minishell.h

SRCS = main.c \
       $(addprefix parsing/, \
	ft_syntax.c ft_syntax_2.c ft_env_expansion.c ft_env_expansion_utils.c) \
       $(addprefix utils/, \
	ft_error.c ft_signal_handler.c ft_isoperator.c ft_space_correction.c ft_space_correction_helper.c ft_space_correction_helper_2.c\
	ft_tokendelone.c ft_input.c ft_freexit.c) \
       $(addprefix tokenization/, \
	ft_tokenization.c ft_tokenization_helper.c ft_syntax_tree.c ft_tree_node.c ft_node_ctor_dtor.c ft_make_tree.c) \
       $(addprefix builtin/, \
    builtin_utils.c builtin_write.c cd.c echo.c env.c exit.c export.c pwd.c unset.c)

OBJ_SUBDIRS = $(addprefix $(OBJ_DIR)/, \
		builtin parsing utils tokenization)

OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

CC = cc
CFLAGS = -Wall -Wextra -Werror -Ireadline
INC = -Ireadline -I$(LIBFT_DIR)
LIBFT = $(LIBFT_DIR)/libft.a

RM = rm -f

all: $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
	mkdir -p $(OBJ_SUBDIRS)

$(NAME): $(OBJS) $(LIBFT) $(DEPS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

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
