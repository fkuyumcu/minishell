NAME = minishell

CC = cc
CFLAGS =  -g #-fsanitize=address #-Wall -Wextra -Werror
SRC = src/main.c \
      src/parser/parser.c \
      src/utils/print_banner.c \
      src/utils/utils.c \
      src/utils/utils2.c \
      src/utils/utils3.c \
      src/utils/error.c \
      src/parser/quotes.c \
      src/parser/lexical_analysis.c \
      src/parser/env_check.c \
      src/parser/create_ast.c \
      src/exec/execute.c \
      src/exec/exec_word.c \
      src/exec/exec_heredoc.c \
      src/exec/find_heredoc.c \
      src/parser/create_line.c \
      

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
