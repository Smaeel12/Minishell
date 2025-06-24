CC = cc
NAME = minishell
INCLUDE = include/main.h
CFLAGS = -Wall -Wextra -Werror -g3 #-fsanitize=address
SRCS = minishell.c $(addprefix src/, parser.c execute.c builtins.c signals.c\
					env_utils.c parser_utils.c execute_utils.c builtins_add.c\
					resource_cleanup.c) debug.c
OBJS = $(SRCS:%.c=objs/%.o)

objs/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJS) $(INCLUDE)
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

.SECONDARY: $(OBJS)