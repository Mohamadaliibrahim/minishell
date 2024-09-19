CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -Iinc/ -Ilibft/

NAME = minishell

SRC = src/main.c src/tokenize/tokenize.c src/tokenize/tokenize_check.c \
      src/tokenize/tokenize_tools.c src/tokenize/utils.c \
      src/parsing/cmd.c src/parsing/echo.c src/parsing/pwd_cd.c \
	  src/parsing/dollar.c src/tokenize/env_tokenizeing.c\
	  src/parsing/ft_env.c src/parsing/ft_export.c\
	  src/parsing/sort_export.c src/parsing/update_pwd.c\
	  src/parsing/ft_export_update.c src/parsing/ft_unset.c\
	  src/parsing/signals.c

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