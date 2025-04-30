NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 #-fsanitize=address
SRCS = main.c
OBJS = $(addprefix objs/, $(SRCS:.c=.o))
HEADER = main.h

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -Llibft -lft -lreadline -ltermcap

objs/%.o: %.c $(HEADER)
	@mkdir -p objs/
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf objs/

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
