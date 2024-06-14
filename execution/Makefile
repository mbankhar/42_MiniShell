NAME	:= builtin

LIBFT	:= libft
INCLUDES := #-I $(LIBFT)
# INCLUDES := -I /libft/libft.h
LIBFT_LIB	:= $(LIBFT)/libft.a
SRCS	:= get_path.c execute.c
OBJS	:= ${SRCS:.c=.o}

# CFLAGS	:= -Wextra -Wall -Werror -g

all:  $(NAME)


libmake:
	$(MAKE) -C $(LIBFT)


%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES) && printf "Compiling: $(notdir $<)"

$(NAME): libmake $(OBJS)
	$(CC) $(OBJS) -Llibft -lft $(LIBS)  $(INCLUDES) -o $(NAME)



clean:
	@rm -rf $(OBJS)
	@rm -rf $(LIBFT)/*.o

fclean: clean
	@rm -rf $(NAME)
	@rm -rf $(LIBFT)/*.a

re: clean all

.PHONY: all, clean, fclean, re
