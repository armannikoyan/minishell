NAME = minishell

SRC_DIR = src
OBJ_DIR = obj
LIBFT_DIR = libs/libft

DEPS = includes/minishell.h

SRCS = main.c \
       $(addprefix parsing/, \
       ft_syntax.c ft_env_expansion.c ft_env_expansion_utils.c) \
       $(addprefix utils/, \
       ft_error.c ft_signal_handler.c ft_isoperator.c ft_space_correction.c \
       ft_tokendelone.c ft_report_error.c) \
       $(addprefix tokenization/, \
       ft_tokenization.c ft_syntax_tree.c ft_execution.c ft_argument_manipulation.c) \

OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -Ireadline
INC = -Ireadline -I$(LIBFT_DIR)
LIBFT = $(LIBFT_DIR)/libft.a

RM = rm -f

all: $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(NAME): $(OBJS) $(LIBFT) $(DEPS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)

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

# NAME = minishell

# SRC_DIR = src
# OBJ_DIR = obj
# LIBFT_DIR = libs/libft

# DEPS = includes/minishell.h

# SRCS = main.c \
#        $(addprefix parsing/, \
#        ft_syntax.c ft_env_expansion.c ft_env_expansion_utils.c) \
#        $(addprefix utils/, \
#        ft_error.c ft_signal_handler.c ft_isoperator.c ft_space_correction.c \
#        ft_tokendelone.c ft_report_error.c) \
#        $(addprefix tokenization/, \
#        ft_tokenization.c ft_syntax_tree.c ft_execution.c ft_argument_manipulation.c) \

# OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

# CC = cc
# CFLAGS = -Wall -Wextra -Werror
# INC = -I/usr/local/Cellar/readline/8.2.13/include -I$(LIBFT_DIR)
# LIBFT = $(LIBFT_DIR)/libft.a
# RL_LIB = -L/usr/local/Cellar/readline/8.2.13/lib -lreadline

# RM = rm -f

# all: $(NAME)

# $(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
# 	@mkdir -p $(@D)
# 	$(CC) $(CFLAGS) $(INC) -c $< -o $@

# $(NAME): $(OBJS) $(LIBFT) $(DEPS)
# 	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(RL_LIB) -o $(NAME)

# $(LIBFT):
# 	make -C $(LIBFT_DIR) all
# 	make -C $(LIBFT_DIR) bonus

# clean:
# 	$(RM) -r $(OBJ_DIR)
# 	make -C $(LIBFT_DIR) clean

# fclean: clean
# 	$(RM) $(NAME)
# 	make -C $(LIBFT_DIR) fclean

# re: fclean all

# bonus: all

# .PHONY: all bonus clean fclean re
