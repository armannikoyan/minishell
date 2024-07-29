# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dasargsy <dasargsy@student.42yerevan.am    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/29 22:45:39 by anikoyan          #+#    #+#              #
#    Updated: 2024/07/29 23:40:27 by anikoyan         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRC_DIR = src
UTILS_DIR = utils
OBJ_DIR = obj
LIB_DIR = libs
LIBFT_DIR = $(LIB_DIR)/libft
READLINE_TAR = $(LIB_DIR)/readline.tar
READLINE_DIR = $(LIB_DIR)/readline

SRC_FILES = main.c node.c
UTILS_FILES = formatters.c

SRC = $(addprefix $(SRC_DIR)/, $(SRC_FILES))
UTILS = $(addprefix $(SRC_DIR)/$(UTILS_DIR)/, $(UTILS_FILES))
ALL_SRC = $(SRC) $(UTILS)

OBJ = $(addprefix $(OBJ_DIR)/, $(patsubst %.c, %.o, $(notdir $(ALL_SRC))))

CC = cc
CFLAGS = -Wall -Wextra -Werror
INC = -I $(LIBFT_DIR)

LIBFT = $(LIBFT_DIR)/libft.a
READLINE = $(READLINE_DIR)/libreadline.a

RM = rm -f

all: $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/$(UTILS_DIR)/%.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(NAME): $(OBJ_DIR) $(OBJ) $(LIBFT) $(READLINE)
	$(CC) $(OBJ) $(LIBFT) $(READLINE) -o $(NAME)

$(READLINE):
	tar -xvf $(READLINE_TAR) -C $(LIB_DIR)
	cd $(READLINE_DIR) && ./configure && make
	cd ../../

$(LIBFT):
	make -C $(LIBFT_DIR)

clean:
	$(RM) -r $(OBJ_DIR)
	make -C $(LIBFT_DIR) clean
	make -C $(READLINE_DIR) clean

fclean: clean
	$(RM) $(NAME)
	$(RM) -r $(READLINE_DIR)
	make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
