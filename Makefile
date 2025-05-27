CC = cc
NAME = minishell
INCLUDE = include/main.h
CFLAGS = -Wall -Wextra -Werror -g3 #-fsanitize=address
SRCS = $(addprefix src/, parser.c parser_utils.c execute.c) main.c
OBJS = $(SRCS:%.c=objs/%.o)

objs/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -Llibft -lft -lreadline -ltermcap


clean:
	rm -rf objs/

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
