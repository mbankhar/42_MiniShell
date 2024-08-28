NAME    := minishell

LIBFT   := libft
INCLUDES := -I $(LIBFT)
LIBFT_LIB := $(LIBFT)/libft.a
VPATH   :=
SRCS    := main.c split.c quotes_check.c get_path.c execute.c \
token.c parse.c builtin_functions.c export.c expand.c utils.c signals.c change_directory.c redirection.c heredoc.c exit.c export_helpers1.c export_helpers2.c export_helpers3.c export_helpers4.c parse_helpers.c helpers/utils_helpers.c helpers/builtin_helpers.c helpers/change_directory_helpers.c helpers/execute_helpers.c  helpers/main_helpers.c helpers/main_helpers2.c helpers/split_helpers1.c helpers/split_helpers2.c helpers/token_helpers.c helpers/expand_helpers.c helpers/echo_helpers.c helpers/update_env_helpers.c helpers/utils_helpers2.c helpers/main_helpers3.c helpers/builtin_helpers1.c helpers/execute_helpers1.c  helpers/special_char_helpers.c helpers/builtin_helpers2.c redirection_helpers.c helpers/execute_helpers01.c helpers/execute_helpers02.c helpers/execute_helpers03.c helpers/execute_helpers04.c helpers/exit_helpers.c process_helpers.c helpers/parse_helpers5.c helpers/funny_helpers.c parse_helpers2.c parse_helpers3.c

OBJS    := ${SRCS:.c=.o}

CFLAGS  := -Wextra -Wall -Werror -g
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
