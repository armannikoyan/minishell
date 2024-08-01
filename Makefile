# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dasargsy <dasargsy@student.42yerevan.am    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/29 22:45:39 by anikoyan          #+#    #+#              #
<<<<<<< HEAD
#    Updated: 2024/07/30 16:56:54 by anikoyan         ###   ########.fr        #
=======
#    Updated: 2024/07/30 02:19:04 by dasargsy         ###   ########.fr        #
>>>>>>> fd0d604047186a539510f33e11f222f7d38a0b8e
#                                                                              #
# **************************************************************************** #

NAME = minishell

# DEPENDENCY FILERNEM AVELACREL VOR MAKEFILE Y KAM HEADERNERY POXEM TAZUC MAKE LINI

DEPS = Makefile includes/minishell.h includes/tokenization.h includes/prompt.h includes/node.h
SRC_DIR = src
<<<<<<< HEAD
=======
UTILS_DIR = utils
TOKENIZATION_DIR = tokenization
PROMPT_DIR = prompt
>>>>>>> fd0d604047186a539510f33e11f222f7d38a0b8e
OBJ_DIR = obj
LIB_DIR = libs
LIBFT_DIR = $(LIB_DIR)/libft
READLINE_DIR = $(LIB_DIR)/readline

DEP = includes/minishell.h includes/node.h
SRC_FILES = main.c node.c
UTILS_FILES = formatters.c
<<<<<<< HEAD

SRC = $(addprefix $(SRC_DIR)/, $(SRC_FILES))
SRC += $(addprefix $(SRC_DIR)/utils/, $(UTILS_FILES))
=======
TOKENIZATION_FILES = tokenization.c token_utils.c
PROMPT_FILES = prompt.c prompt_utils.c

SRC = $(addprefix $(SRC_DIR)/, $(SRC_FILES))
UTILS = $(addprefix $(SRC_DIR)/$(UTILS_DIR)/, $(UTILS_FILES))
TOKENIZATION = $(addprefix $(SRC_DIR)/$(TOKENIZATION_DIR)/, $(TOKENIZATION_FILES))
PROMPT = $(addprefix $(SRC_DIR)/$(PROMPT_DIR)/, $(PROMPT_FILES))

ALL_SRC = $(SRC) $(UTILS) $(TOKENIZATION) $(PROMPT)
>>>>>>> fd0d604047186a539510f33e11f222f7d38a0b8e

OBJ = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))

CC = cc
CFLAGS = -Wall -Wextra -Werror
<<<<<<< HEAD
=======

>>>>>>> fd0d604047186a539510f33e11f222f7d38a0b8e
INC = -I $(LIBFT_DIR) -I $(READLINE_DIR)

LIBFT = $(LIBFT_DIR)/libft.a
READLINE = $(READLINE_DIR)/libreadline.a

RM = rm -f

all: $(NAME)

$(READLINE):
<<<<<<< HEAD
	tar -xvf $(LIB_DIR)/readline.tar -C $(LIB_DIR)
	cd $(READLINE_DIR) && ./configure && make
	cd ../../

$(LIBFT):
	make -C $(LIBFT_DIR)

=======
	tar -xvf $(READLINE_TAR) -C $(LIB_DIR)
	cd $(READLINE_DIR) && ./configure && make
	cd ../../

>>>>>>> fd0d604047186a539510f33e11f222f7d38a0b8e
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/$(UTILS_DIR)/%.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/$(TOKENIZATION_DIR)/%.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

<<<<<<< HEAD
$(NAME): $(READLINE) $(LIBFT) $(OBJ_DIR) $(OBJ) $(DEP)
	$(CC) $(OBJ) $(LIBFT) -lreadline $(READLINE) -o $(NAME)
=======
$(OBJ_DIR)/%.o: $(SRC_DIR)/$(PROMPT_DIR)/%.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

# SKZBIC PETQA READLINE Y MAKE LINI VOR MINISHELLY KARANA IRA HEADERY QTNI

$(NAME):$(READLINE) $(OBJ_DIR) $(OBJ) $(LIBFT) $(DEPS)
	$(CC) $(OBJ) $(LIBFT) -lreadline $(READLINE) -o $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)
>>>>>>> fd0d604047186a539510f33e11f222f7d38a0b8e

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
