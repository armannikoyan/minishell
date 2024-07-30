# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dasargsy <dasargsy@student.42yerevan.am    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/29 22:45:39 by anikoyan          #+#    #+#              #
#    Updated: 2024/07/30 16:56:54 by anikoyan         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRC_DIR = src
OBJ_DIR = obj
LIB_DIR = libs
LIBFT_DIR = $(LIB_DIR)/libft
READLINE_DIR = $(LIB_DIR)/readline

DEP = includes/minishell.h includes/node.h
SRC_FILES = main.c node.c
UTILS_FILES = formatters.c

SRC = $(addprefix $(SRC_DIR)/, $(SRC_FILES))
SRC += $(addprefix $(SRC_DIR)/utils/, $(UTILS_FILES))

OBJ = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))

CC = cc
CFLAGS = -Wall -Wextra -Werror
INC = -I $(LIBFT_DIR) -I $(READLINE_DIR)

LIBFT = $(LIBFT_DIR)/libft.a
READLINE = $(READLINE_DIR)/libreadline.a

RM = rm -f

all: $(NAME)

$(READLINE):
	tar -xvf $(LIB_DIR)/readline.tar -C $(LIB_DIR)
	cd $(READLINE_DIR) && ./configure && make
	cd ../../

$(LIBFT):
	make -C $(LIBFT_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/$(UTILS_DIR)/%.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/$(TOKENIZATION_DIR)/%.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(NAME): $(READLINE) $(LIBFT) $(OBJ_DIR) $(OBJ) $(DEP)
	$(CC) $(OBJ) $(LIBFT) -lreadline $(READLINE) -o $(NAME)

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
