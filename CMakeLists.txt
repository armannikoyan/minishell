cmake_minimum_required(VERSION 3.21)

project(minishell C)

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
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/includes)

# ===============================
# Sources
# ===============================
set(SOURCES
        ${SRC_DIR}/main.c
        ${SRC_DIR}/minishell.c

        ${SRC_DIR}/utils/error.c
        ${SRC_DIR}/utils/signals.c
        ${SRC_DIR}/utils/term_config.c
        ${SRC_DIR}/utils/cursor.c

        ${SRC_DIR}/hash_table/hash_table.c
        ${SRC_DIR}/hash_table/hash_table_utils.c

        ${SRC_DIR}/ast/ast.c
        ${SRC_DIR}/ast/ast_utils.c

        ${SRC_DIR}/tokenization/tokenization.c
        ${SRC_DIR}/tokenization/tokenization_utils.c
)

# ===============================
# libft (CMake-native)
# ===============================
add_subdirectory(libs/libft)

# ===============================
# Executable
# ===============================
add_executable(minishell ${SOURCES})

target_include_directories(minishell PRIVATE
        ${INCLUDE_DIR}
)

target_link_libraries(minishell PRIVATE
        ft
)

# ===============================
# Readline & platform-specific
# ===============================
if(APPLE)
    execute_process(
            COMMAND brew --prefix readline
            OUTPUT_VARIABLE READLINE_PREFIX
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    target_include_directories(minishell PRIVATE
            ${READLINE_PREFIX}/include
    )

    target_link_directories(minishell PRIVATE
            ${READLINE_PREFIX}/lib
    )
endif()

if(UNIX AND NOT APPLE)
    target_link_libraries(minishell PRIVATE ncurses)
endif()

target_link_libraries(minishell PRIVATE readline)
