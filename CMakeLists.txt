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
        src/term_settings/signals.c
        src/term_settings/term_config.c

        src/main.c
        src/minishell.c

        src/utils/error.c

        src/hash_table/hash_table.c
        src/hash_table/hash_table_utils.c

        src/ast/ast.c
        src/ast/ast_node_utils.c
        src/ast/ast_print_module.c
        src/ast/ast_print_module_utils.c

        src/tokenization/tokenization.c
        src/tokenization/tokenization_utils.c
        src/tokenization/tokenization_node_creation_module.c

        src/builtin/pwd.c
        src/builtin/exit.c
        src/builtin/env.c
        src/builtin/echo.c
        src/path_utils/path_utils.c
        src/builtin/cd/cd.c
        src/builtin/cd/cd_path_utils.c
        src/builtin/cd/cd_path_parser.c
        src/builtin/cd/cd_path_resolver.c
        src/builtin/export.c
        src/builtin/unset.c

        src/execution/execution.c
        src/execution/execution_utils.c
        src/execution/execution_redir_node.c
        src/execution/execution_binary_node.c
        src/execution/execution_command_node.c
        src/execution/expansion/dollar_sign_expansion.c
        src/execution/expansion/dollar_sign_expansion_utils.c
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
