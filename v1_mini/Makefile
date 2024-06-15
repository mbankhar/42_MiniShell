# Variables
CC = gcc
CFLAGS = -Wall -Wextra -Werror -I/usr/local/opt/readline/include -I/usr/include -I./parsing
LDFLAGS = -L/usr/local/opt/readline/lib -L/usr/lib
# LIBFT = ./libft/libft.a
LIBS = -lreadline

# Executable
NAME = minishell

# Source Files
SRC = main.c token_utils.c tokenize_input.c \
      parsing/parse_args.c parsing/parse_cmd.c parsing/parse_d_quotes.c \
      parsing/parse_error.c parsing/free_memory.c parsing/parse_options.c \
      parsing/parse_pipes.c parsing/parse_redir.c parsing/parse_redr_out.c \
      parsing/parse_sep.c parsing/syntax_err.c parsing/parse_var_env.c \
      parsing/var_env_utils.c

# Object Files
OBJ = $(SRC:.c=.o)

# Rules
all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJ) $(LIBFT) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
