NAME = minishell

SRC_DIR = src
OBJ_DIR = obj
LIBFT_DIR = libs/libft

DEPS = includes/minishell.h

SRCS = main.c ft_utils.c \
       $(addprefix parsing/, \
       ft_syntax.c ft_env_expansion.c ft_env_expansion_utils.c \
	   ft_parsing_utils.c) \
       $(addprefix utils/, \
       ft_error.c ft_signal_handler.c ft_isoperator.c ft_space_correction.c \
       ft_tokendelone.c ft_report_error.c ft_space_correction_utils.c) \
       $(addprefix tokenization/, \
       ft_tokenization.c ft_syntax_tree.c ft_execution.c ft_argument_manipulation.c \
	   ft_tok_utils.c ft_tok_utils2.c ft_argument_quote.c ft_argument_process.c \
	   ft_argument_utils.c  ft_syntax_tree_parse.c ft_syntax_node.c \
	   ft_exectuion_pipe.c ft_execution_command.c ft_execution_errors.c \
	   ft_execution_heredoc.c ft_execution_redirections.c) \
       $(addprefix bin/, \
       builtin_utils.c builtin_write.c cd.c echo.c env.c exit.c export.c export_var.c \
	   pwd.c unset.c) \

OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror
INC = -I$(LIBFT_DIR) -I../readline/readline
LIBFT = $(LIBFT_DIR)/libft.a
RL_LIB = ../readline/libreadline.a -lncurses

RM = rm -f

all: $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(NAME): $(OBJS) $(LIBFT) $(DEPS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(RL_LIB) -o $(NAME)

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
#        $(addprefix bin/, \
#        builtin_utils.c builtin_write.c cd.c echo.c env.c exit.c export.c pwd.c unset.c) \

# OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

# CC = cc
# CFLAGS = -Wall -Wextra -Werror -Ireadline
# INC = -Ireadline -I$(LIBFT_DIR)
# LIBFT = $(LIBFT_DIR)/libft.a

# RM = rm -f

# all: $(NAME)

# $(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
# 	@mkdir -p $(@D)
# 	$(CC) $(CFLAGS) $(INC) -c $< -o $@

# $(NAME): $(OBJS) $(LIBFT) $(DEPS)
# 	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)

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
