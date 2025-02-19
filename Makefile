NAME = minishell

CC = cc
CFLAGS =  #-Wall -Wextra -Werror
SRC = src/main.c \
      src/parser/parser.c \
      src/utils/print_banner.c \
      src/utils/utils.c \
      src/utils/utils2.c \
      src/parser/quotes.c \
      src/parser/lexical_analysis.c \

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