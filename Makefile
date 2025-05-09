NAME = minishell

CC = cc
CFLAGS =  -g  -Wall -Wextra -Werror
SRC = src/main.c \
      src/main_utils.c \
      src/signals.c \
      src/parser/parser.c \
      src/parser/cnt_token.c \
      src/utils/utils.c \
      src/utils/ft_strjoin.c \
      src/utils/ft_strnstr.c \
      src/utils/ft_strlcat.c \
      src/utils/error.c \
      src/parser/quotes.c \
      src/parser/minidup.c \
      src/parser/lexical_analysis.c \
      src/parser/wordfill.c \
      src/parser/env_check.c \
      src/parser/cnt_dollar.c \
      src/parser/replace_env.c \
      src/parser/create_line.c \
      src/parser/priority.c \
      src/parser/handle_builtin.c \
      src/parser/wait.c \
      src/exec/apply_procs.c\
      src/exec/cnt_args.c\
      src/exec/apply_free.c\
      src/exec/execute_free.c\
      src/exec/execute_utils.c\
      src/exec/pipes_redirects.c\
      src/exec/redir_in_out.c\
      src/exec/apply_redirects.c\
      src/exec/expand_heredoc.c\
      builtin/cd/cd.c\
      builtin/echo.c\
      builtin/pwd.c\
      builtin/unset.c\
      builtin/env.c\
      builtin/builtin_utils.c\
      builtin/utils_etc.c\
      builtin/exit.c\
      builtin/cd/cd_env_utils.c\
      builtin/cd/cd_path_utils.c\
      builtin/cd/cd_actions_utils.c\
      builtin/cd/free_cd.c\
      src/parser/ft_itoa.c\
      src/parser/cpy_node.c\
      builtin/export/export.c \
      builtin/export/swp_env.c \
      builtin/export/export_utils.c \
      builtin/export/env_utils.c \
      builtin/export/special_char.c \

OBJ = $(SRC:.c=.o)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	@$(CC) $(OBJ) $(CFLAGS) -o $(NAME) -lreadline

all: $(NAME)

clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re