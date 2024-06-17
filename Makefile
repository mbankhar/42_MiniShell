NAME    := minishell

LIBFT   := libft
INCLUDES := -I $(LIBFT) # This is assuming you need to include libft headers
LIBFT_LIB := $(LIBFT)/libft.a
SRCS    := main.c split.c quotes_check.c
OBJS    := ${SRCS:.c=.o}

# CFLAGS  := -Wextra -Wall -Werror -g
LIBS    := -lreadline -lncurses

all: $(NAME)

libmake:
	$(MAKE) -C $(LIBFT)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES) && printf "Compiling: $(notdir $<)\n"

$(NAME): libmake $(OBJS)
	$(CC) $(OBJS) -L$(LIBFT) -lft $(LIBS) $(INCLUDES) -o $(NAME)

clean:
	@rm -rf $(OBJS)
	@$(MAKE) -C $(LIBFT) clean

fclean: clean
	@rm -rf $(NAME)
	@$(MAKE) -C $(LIBFT) fclean

re: fclean all

.PHONY: all clean fclean re
