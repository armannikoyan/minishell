CFLAGS := -Wall -Wextra -Werror -I$(LIBFT_DIR) ${CPPFLAGS}
LDFLAGS := ${LDFLAGS}
LIBS = -lreadline

NAME = minishell
SRC_DIR = src
OBJ_DIR = obj
LIBFT_DIR = libs/libft
DEPS = includes/minishell.h

LIBFT = $(LIBFT_DIR)/libft.a

SRCS = main.c \
       $(addprefix parsing/, \
       ft_syntax.c ft_env_expansion.c ft_env_expansion_utils.c \
       ft_parsing_utils.c ft_env_pars.c) \
       $(addprefix utils/, \
       ft_error.c ft_signal_handler.c ft_isoperator.c \
       ft_space_correction.c ft_utils.c \
       ft_tokendelone.c ft_report_error.c ft_space_correction_utils.c \
       ft_space_correction_utils2.c ft_space_correctiion_utils3.c ) \
       $(addprefix tokenization/, \
       ft_tokenization.c ft_syntax_tree.c ft_execution.c \
       ft_argument_manipulation.c ft_tok_utils.c ft_tok_utils2.c \
       ft_argument_quote.c ft_argument_process.c ft_argument_utils.c \
       ft_syntax_tree_parse.c ft_syntax_node.c ft_exectuion_pipe.c \
       ft_execution_command.c ft_execution_errors.c ft_execution_heredoc.c \
       ft_execution_redirections.c ft_syntax_node_2.c ft_syntax_tree_2.c \
       ft_argment_quote_helper.c ft_execution_command2.c ft_exec_helper.c \
       ft_exec_redir_helper.c ft_exec_pipe_helper.c ) \
       $(addprefix bin/, \
       builtin_utils.c builtin_write.c cd.c \
       echo.c env.c exit.c \
       export.c export_var.c pwd.c \
       unset.c) \


OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

CC = cc
RM = rm -f

all: $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS) $(LIBFT) $(DEPS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LDFLAGS) $(LIBS) -o $(NAME)

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
