# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/01 23:53:16 by anikoyan          #+#    #+#              #
#    Updated: 2024/07/23 13:26:06 by anikoyan         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

SRC_DIR = src
SRCS = $(addprefix $(SRC_DIR)/, \
       ft_isalpha.c ft_isdigit.c ft_isalnum.c \
       ft_isascii.c ft_isprint.c ft_strlen.c \
       ft_memset.c ft_bzero.c ft_memcpy.c \
       ft_memmove.c ft_strlcpy.c ft_strlcat.c \
       ft_toupper.c ft_tolower.c ft_strchr.c \
       ft_strrchr.c ft_strncmp.c ft_memchr.c \
       ft_memcmp.c ft_strnstr.c ft_atoi.c \
       ft_calloc.c ft_strdup.c ft_substr.c \
       ft_strjoin.c ft_strtrim.c ft_split.c \
       ft_itoa.c ft_strmapi.c ft_striteri.c \
       ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c \
       ft_putnbr_fd.c ft_strrev.c ft_putchar.c \
       ft_putchar_n.c ft_putstr.c ft_uitoa.c \
       ft_uitoa_base.c ft_printf.c ft_strcmp.c \
       ft_isspace.c ft_atof.c\
       $(addprefix utils/, \
       $(addprefix printf/, \
       ft_print_char.c ft_print_string.c ft_print_unsigned.c \
       ft_print_int.c ft_print_hex.c ft_print_pointer.c \
       ft_print_number_flags.c ft_print_parse_flags.c ft_printf_utils.c\
       ) \
       ) \
       )
OBJ_DIR = obj
OBJS = $(addprefix $(OBJ_DIR)/,$(notdir $(SRCS:.c=.o)))

SRC_BONUS_DIR = src_bonus
SRCS_BONUS = $(addprefix $(SRC_BONUS_DIR)/, \
	     ft_lstnew.c ft_lstadd_front.c ft_lstsize.c \
	     ft_lstlast.c ft_lstadd_back.c ft_lstdelone.c \
	     ft_lstclear.c ft_lstiter.c ft_lstmap.c)
OBJS_BONUS = $(addprefix $(OBJ_DIR)/,$(notdir $(SRCS_BONUS:.c=.o)))

CC = cc
C_FLAGS = -Wall -Wextra -Werror
ARFLAGS = rc
RM = rm -f

all: $(OBJ_DIR) $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(NAME): $(OBJS)
	ar $(ARFLAGS) $(NAME) $(OBJS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/utils/printf/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

bonus: $(OBJ_DIR) $(OBJS_BONUS)
	ar $(ARFLAGS) $(NAME) $(OBJS_BONUS)

$(OBJ_DIR)/%.o: $(SRC_BONUS_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(C_FLAGS) -c $< -o $@

clean:
	rm -r $(OBJ_DIR) 2>/dev/null || true

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all bonus clean fclean re
