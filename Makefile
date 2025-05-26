NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 #-fsanitize=address
SRCS = $(addprefix src/, parser.c parser_helpers.c execute_pipeline.c main.c)
OBJS = $(addprefix objs/, $(SRCS:src/%.c=%.o))
INCLUDE = include/main.h

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -Llibft -lft -lreadline -ltermcap

objs/%.o: src/%.c $(INCLUDE)
	@mkdir -p objs/
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf objs/

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
