CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -Iinc/ -Ilibft/

NAME = minishell

SRC = src/main.c \
      src/tokenize/tokenize.c \
      src/tokenize/checking_token/building.c \
      src/tokenize/checking_token/checking_utils.c \
      src/tokenize/checking_token/checking.c \
      src/tokenize/checking_token/tokenize_preprocess.c \
      src/tokenize/checking_token/fix_pipe.c \
      src/tokenize/tokenize_tools.c \
      src/tokenize/utils.c \
      src/parsing/cmd.c \
      src/build_ins/echo.c \
      src/build_ins/pwd_cd/ft_cd.c \
      src/build_ins/pwd_cd/pwd.c\
      src/build_ins/pwd_cd/utils_pwd_cd.c\
      src/build_ins/pwd_cd/env_utils_editing_pwd.c\
      src/build_ins/pwd_cd/geting_path.c\
      src/build_ins/pwd_cd/geting_path_utils.c \
      src/parsing/dollar.c \
      src/tokenize/env/env_tokenizeing.c \
      src/tokenize/env/env_utils.c \
      src/tokenize/env/filling_env.c \
      src/build_ins/ft_env.c \
      src/build_ins/ft_export.c \
      src/build_ins/sort_export.c \
      src/build_ins/update_pwd.c \
      src/build_ins/export_edit/ft_export_update.c \
      src/build_ins/export_edit/edit_env_node.c \
      src/build_ins/export_edit/expanding.c \
      src/build_ins/export_edit/utils_export_edit.c \
      src/build_ins/ft_unset.c \
      src/parsing/signals.c \
      src/execution/exec.c \
      src/redirections/ft_redirections.c \
      src/redirections/redirection_checking.c \
      src/redirections/redirection_utils.c \
      src/redirections/redirection_t_a.c \
      src/tokenize/redirections.c \
      src/execution/pipe.c \
      src/execution/pipe_utils.c \
      src/parsing/expand.c \
      src/tokenize/process_token.c \
      src/execution/allocate_arguments.c \
      src/execution/execute_command.c \
      src/execution/handle_parent_process.c \
      src/execution/parse_commands.c \
      src/execution/pipe_tools.c \
      src/parsing/cmd_tools.c \
      src/execution/env_list.c \
      src/preparing.c \
      src/tokenize/expand_heredoc.c \
      src/tokenize/handle_heredoc_file.c \
      src/tokenize/herdoc_tools.c \
      src/tokenize/parse_heredoc.c

OBJS = $(SRC:.c=.o)

LIBFT = libft/libft.a

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@echo "\033[0;32mBuilding $(NAME)...\033[0m"
	@$(CC) $(CFLAGS) $(OBJS) -Llibft -lft -lreadline -o $(NAME)
	@echo "\033[0;32m$(NAME) built successfully!\033[0m"

$(LIBFT):
	@echo "\033[0;32mBuilding libft...\033[0m"
	@$(MAKE) -C libft > /dev/null
	@echo "\033[0;32mLibft built successfully!\033[0m"

%.o: %.c
	@echo "\033[0;32mCompiling $<...\033[0m"
	@$(CC) $(CFLAGS) -c $< -o $@ > /dev/null

clean:
	@echo "\033[0;32mCleaning up object files...\033[0m"
	@rm -f $(OBJS)
	@$(MAKE) -C libft clean > /dev/null
	@echo "\033[0;32mObject files removed.\033[0m"

fclean: clean
	@echo "\033[0;32mRemoving $(NAME) and libft...\033[0m"
	@rm -f $(NAME)
	@$(MAKE) -C libft fclean > /dev/null
	@echo "\033[0;32m$(NAME) and libft removed.\033[0m"

re: fclean all

.PHONY: all clean fclean re
