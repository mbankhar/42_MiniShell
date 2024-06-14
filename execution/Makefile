# Variables
CC = gcc
CFLAGS = -Wall -Wextra -Werror
LIBFT = ./libft/libft.a
LIBS = -lreadline

# Executable
NAME = minishell

# Source Files
SRC = main.c get_path.c execute.c expand.c

# Object Files
OBJ = $(SRC:.c=.o)

# Rules
all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LIBFT) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	$(MAKE) -C ./libft

clean:
	$(RM) $(OBJ)
	$(MAKE) -C ./libft clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C ./libft fclean

re: fclean all

.PHONY: all clean fclean re
