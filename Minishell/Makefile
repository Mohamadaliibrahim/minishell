CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -Iinc/ -Ilibft/

NAME = minishell

SRC = src/main.c \
      src/preparing.c \
      src/build_ins/export_edit/edit_env_node.c \
      src/build_ins/export_edit/expanding.c \
      src/build_ins/export_edit/ft_export_update.c \
      src/build_ins/export_edit/utils_export_edit.c \
      src/build_ins/pwd_cd/env_utils_editing_pwd.c \
      src/build_ins/pwd_cd/cd.c \
      src/build_ins/pwd_cd/getting_path.c \
      src/build_ins/pwd_cd/getting_path_utils.c \
      src/build_ins/pwd_cd/pwd.c \
      src/build_ins/pwd_cd/utils_pwd_cd.c \
      src/build_ins/echo.c \
      src/build_ins/env.c \
      src/build_ins/export.c \
      src/build_ins/sort_export.c \
      src/build_ins/unset.c \
      src/build_ins/update_pwd.c \
      src/execution/do_command/allocate_arguments.c \
      src/execution/do_command/env_list.c \
      src/execution/do_command/exec.c \
      src/execution/do_command/execute_command.c \
      src/execution/do_command/handle_parent_process.c \
      src/execution/pipe/parse_commands.c \
      src/execution/pipe/pipe_tools.c \
      src/execution/pipe/pipe_utils.c \
      src/execution/pipe/pipe.c \
      src/parsing/cmd_tools.c \
      src/parsing/cmd.c \
      src/parsing/dollar.c \
      src/parsing/expand.c \
      src/parsing/signals.c \
      src/redirections/ft_redirections.c \
      src/redirections/redirection_checking.c \
      src/redirections/redirection_t_a.c \
      src/redirections/redirection_utils.c \
      src/redirections/redirections.c \
      src/tokenize/checking_token/building.c \
      src/tokenize/checking_token/checking_utils.c \
      src/tokenize/checking_token/checking.c \
      src/tokenize/checking_token/fix_pipe.c \
      src/tokenize/checking_token/tokenize_preprocess.c \
      src/tokenize/env/env_tokenizeing.c \
      src/tokenize/env/env_utils.c \
      src/tokenize/env/filling_env.c \
      src/tokenize/heredoc/expand_heredoc.c \
      src/tokenize/heredoc/handle_heredoc_file.c \
      src/tokenize/heredoc/heredoc_tools.c \
      src/tokenize/heredoc/parse_heredoc.c \
      src/tokenize/token/process_token.c \
      src/tokenize/token/tokenize_tools.c \
      src/tokenize/token/tokenize.c \
      src/tokenize/token/utils.c \

      

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
