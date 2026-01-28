cmake_minimum_required(VERSION 3.21)

project(libft C)

# ===============================
# Standard
# ===============================
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# ===============================
# Compiler flags
# ===============================
add_compile_options(-Wall -Wextra -Werror)

# ===============================
# Paths
# ===============================
set(LIBFT_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(LIBFT_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/includes)

# ===============================
# Base sources
# ===============================
set(LIBFT_SOURCES
        ${LIBFT_SRC_DIR}/ft_isalpha.c
        ${LIBFT_SRC_DIR}/ft_isdigit.c
        ${LIBFT_SRC_DIR}/ft_isalnum.c
        ${LIBFT_SRC_DIR}/ft_isascii.c
        ${LIBFT_SRC_DIR}/ft_isprint.c
        ${LIBFT_SRC_DIR}/ft_strlen.c
        ${LIBFT_SRC_DIR}/ft_memset.c
        ${LIBFT_SRC_DIR}/ft_bzero.c
        ${LIBFT_SRC_DIR}/ft_memcpy.c
        ${LIBFT_SRC_DIR}/ft_memmove.c
        ${LIBFT_SRC_DIR}/ft_strlcpy.c
        ${LIBFT_SRC_DIR}/ft_strlcat.c
        ${LIBFT_SRC_DIR}/ft_toupper.c
        ${LIBFT_SRC_DIR}/ft_tolower.c
        ${LIBFT_SRC_DIR}/ft_strchr.c
        ${LIBFT_SRC_DIR}/ft_strrchr.c
        ${LIBFT_SRC_DIR}/ft_strncmp.c
        ${LIBFT_SRC_DIR}/ft_memchr.c
        ${LIBFT_SRC_DIR}/ft_memcmp.c
        ${LIBFT_SRC_DIR}/ft_strnstr.c
        ${LIBFT_SRC_DIR}/ft_atoi.c
        ${LIBFT_SRC_DIR}/ft_calloc.c
        ${LIBFT_SRC_DIR}/ft_strdup.c
        ${LIBFT_SRC_DIR}/ft_substr.c
        ${LIBFT_SRC_DIR}/ft_strjoin.c
        ${LIBFT_SRC_DIR}/ft_strtrim.c
        ${LIBFT_SRC_DIR}/ft_split.c
        ${LIBFT_SRC_DIR}/ft_itoa.c
        ${LIBFT_SRC_DIR}/ft_strmapi.c
        ${LIBFT_SRC_DIR}/ft_striteri.c
        ${LIBFT_SRC_DIR}/ft_putchar_fd.c
        ${LIBFT_SRC_DIR}/ft_putstr_fd.c
        ${LIBFT_SRC_DIR}/ft_putendl_fd.c
        ${LIBFT_SRC_DIR}/ft_putnbr_fd.c
        ${LIBFT_SRC_DIR}/ft_strrev.c
        ${LIBFT_SRC_DIR}/ft_putchar.c
        ${LIBFT_SRC_DIR}/ft_putchar_n.c
        ${LIBFT_SRC_DIR}/ft_putstr.c
        ${LIBFT_SRC_DIR}/ft_uitoa.c
        ${LIBFT_SRC_DIR}/ft_uitoa_base.c
        ${LIBFT_SRC_DIR}/ft_strcmp.c
        ${LIBFT_SRC_DIR}/ft_isspace.c
        ${LIBFT_SRC_DIR}/ft_atof.c
        ${LIBFT_SRC_DIR}/ft_strcpy.c
        ${LIBFT_SRC_DIR}/ft_strcat.c
        ${LIBFT_SRC_DIR}/ft_sqrt.c
        ${LIBFT_SRC_DIR}/ft_isprime.c
        ${LIBFT_SRC_DIR}/ft_lstnew.c
        ${LIBFT_SRC_DIR}/ft_lstadd_front.c
        ${LIBFT_SRC_DIR}/ft_lstsize.c
        ${LIBFT_SRC_DIR}/ft_lstlast.c
        ${LIBFT_SRC_DIR}/ft_lstadd_back.c
        ${LIBFT_SRC_DIR}/ft_lstdelone.c
        ${LIBFT_SRC_DIR}/ft_lstclear.c
        ${LIBFT_SRC_DIR}/ft_lstiter.c
        ${LIBFT_SRC_DIR}/ft_lstmap.c
)


add_library(ft STATIC ${LIBFT_SOURCES})

target_include_directories(ft PUBLIC
        ${LIBFT_INC_DIR}
)

if(LIBFT_BONUS)
    target_sources(ft PRIVATE ${LIBFT_BONUS_SOURCES})
endif()
