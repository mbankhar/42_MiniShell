NAME    := minishell

LIBFT   := libft
INCLUDES := -I $(LIBFT) # This is assuming you need to include libft headers
LIBFT_LIB := $(LIBFT)/libft.a
SRCS    := main.c split.c quotes_check.c get_path.c execute.c \
number_of_commands.c token_dollars_redirect.c parse.c builtin_functions.c
OBJS    := ${SRCS:.c=.o}

# CFLAGS  := -Wextra -Wall -Werror -g
LIBS    := -lreadline -lncurses

all: $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES) && printf "Compiling: $(notdir $<)\n"

$(NAME): $(LIBFT_LIB) $(OBJS)
	$(CC) $(OBJS) -L$(LIBFT) -lft $(LIBS) $(INCLUDES) -o $(NAME)
	
$(LIBFT_LIB) : $(LIBFT)
	cd libft && make

clean:
	@rm -rf $(OBJS)
	@$(MAKE) -C $(LIBFT) clean

fclean: clean
	@rm -rf $(NAME)
	@$(MAKE) -C $(LIBFT) fclean

re: fclean all

run: all
	./minishell

.PHONY: all clean fclean re run
