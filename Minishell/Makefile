CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -Iinc/ -Ilibft/

NAME = minishell

SRC = src/main.c src/tokonize.c src/utils.c
OBJS = $(SRC:.c=.o)

LIBFT = libft/libft.a

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
		$(CC) $(CFLAGS) $(OBJS) -Llibft -lft -lreadline -o $(NAME)

$(LIBFT):
		@make -C libft

%.o: %.c
		$(CC) $(CFLAGS) -c $< -o $@

clean:
		rm -f $(OBJS)
		@make -C libft clean

fclean: clean
		rm -f $(NAME)
		@make -C libft fclean

re: fclean all

.PHONY: all clean fclean re
