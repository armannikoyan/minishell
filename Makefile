NAME = minishell

# DEPENDENCY FILERNEM AVELACREL VOR MAKEFILE Y KAM HEADERNERY POXEM TAZUC MAKE LINI

DEPS = Makefile includes/minishell.h includes/prompt.h includes/node.h
SRC_DIR = src
UTILS_DIR = utils
PROMPT_DIR = prompt
OBJ_DIR = obj
LIB_DIR = libs
LIBFT_DIR = $(LIB_DIR)/libft
READLINE_DIR = $(LIB_DIR)/readline-8.2
READLINE_TAR = $(LIB_DIR)/readline.gz

DEP = includes/minishell.h includes/node.h
SRC_FILES = main.c node.c
UTILS_FILES = formatters.c
PROMPT_FILES = prompt.c prompt_utils.c 

SRC = $(addprefix $(SRC_DIR)/, $(SRC_FILES))
UTILS = $(addprefix $(SRC_DIR)/$(UTILS_DIR)/, $(UTILS_FILES))
PROMPT = $(addprefix $(SRC_DIR)/$(PROMPT_DIR)/, $(PROMPT_FILES))
ALL_SRC = $(SRC) $(UTILS) $(PROMPT)

OBJ = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))

CC = cc
CFLAGS = -Wall -Wextra -Werror -Ilibs/readline-8.2 -Ilibs/libft
INC = -I $(LIBFT_DIR) -I $(READLINE_DIR)

LIBFT = $(LIBFT_DIR)/libft.a
READLINE = $(READLINE_DIR)/libreadline.a

RM = rm -f

all: $(NAME)

$(READLINE):
	tar xvfz $(READLINE_TAR) -C $(LIB_DIR) && \
	cd $(READLINE_DIR) && \
	./configure --prefix=$(HOME)/readline && \
	make && \
	make install
	cd ../../

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/$(UTILS_DIR)/%.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/$(PROMPT_DIR)/%.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

# SKZBIC PETQA READLINE Y MAKE LINI VOR MINISHELLY KARANA IRA HEADERY QTNI

$(NAME):$(READLINE) $(ALL_SRC) $(LIBFT) $(DEPS)
	$(CC) $(ALL_SRC) $(LIBFT) -lreadline $(READLINE) -o $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

clean:
	$(RM) -r $(OBJ_DIR)
	make -C $(LIBFT_DIR) clean
	make -C $(READLINE_DIR) clean 2> /dev/null | true

fclean: clean
	$(RM) $(NAME)
	make -C $(LIBFT_DIR) fclean
	$(RM) -r $(READLINE_DIR) 2> /dev/null | true

re: fclean all

.PHONY: all clean fclean re
