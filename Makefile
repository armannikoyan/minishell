# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dasargsy <dasargsy@student.42yerevan.am    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/29 18:10:52 by anikoyan          #+#    #+#              #
#    Updated: 2024/07/29 23:05:56 by dasargsy         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRC_DIR = src
UTILS_DIR = utils
OBJ_DIR = obj
LIBFT_DIR = libft

SRC_FILES = main.c
UTILS_FILES = formatters.c

SRC = $(addprefix $(SRC_DIR)/, $(SRC_FILES))
UTILS = $(addprefix $(SRC_DIR)/$(UTILS_DIR)/, $(UTILS_FILES))
ALL_SRC = $(SRC) $(UTILS)

OBJ = $(addprefix $(OBJ_DIR)/, $(patsubst %.c, %.o, $(notdir $(ALL_SRC))))

CC = cc
CFLAGS = -Wall -Wextra -Werror
INC = -I $(LIBFT_DIR)

LIBFT = $(LIBFT_DIR)/libft.a

RM = rm -f

all: $(OBJ_DIR) $(LIBFT) $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/$(UTILS_DIR)/%.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(LIBFT) -o $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

clean:
	$(RM) -r $(OBJ_DIR)
	make -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME)
	make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
