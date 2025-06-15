CC = cc
NAME = minishell
INCLUDE = include/main.h
CFLAGS = -Wall -Wextra -Werror -g3 #-fsanitize=address
SRCS = $(addprefix src/, parser.c parser_utils.c execute.c resource_cleanup.c \
	signals.c builtins.c builtins_add.c environement_utils.c)\
	minishell.c debug.c
OBJS = $(SRCS:%.c=objs/%.o)

objs/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJS)
	make -C libft
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -Llibft -lft -lreadline -ltermcap


clean:
	make clean -C libft
	rm -rf objs/

fclean: clean
	make fclean -C libft
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
