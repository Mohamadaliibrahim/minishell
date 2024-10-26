/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:07:23 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/10/26 15:29:38 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>
# include <ctype.h>
# include <stdio.h>
# include <signal.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>
# include <limits.h>
# include <sys/stat.h>
# include <errno.h>
# include <setjmp.h>

/* Enum for Token Types */
typedef enum e_token_type
{
	CMND,
	PIPE,
	REDIRECT_IN,
	REDIRECT_OUT,
	APPEND,
	HEREDOC,
	EQUAL,
	VARIABLE,
	QUOTE,
	DQUOTE,
	UNKNOWN
}					t_token_type;

/* Struct for Token */
typedef struct s_token
{
	char			*tokens;
	int				token_type;
	char			qoute_type;
	struct s_token	*next;
	struct s_token	*previous;
}					t_token;

typedef struct s_input
{
	char			*new_input;
	int				i;
	int				j;
	int				len;
	char			quote;
	int				new_len;
}					t_input;

typedef struct s_cd
{
	char			*path;
	char			*old_pwd;
	char			*new_pwd;
	char			*pwd_env;
	char			*oldpwd_env;
	char			*hello;
	int				should_free;
}					t_cd;

typedef struct s_export
{
	char			*expanded_env;
	char			*env;
	char			*type;
	bool			equal;
}					t_export;

/* Struct for Environment Copy */
typedef struct s_env_cpy
{
	char				*env;
	char				*type;
	char				*heredoc_file;
	bool				equal;
	int					last_exit_status;
	int					last_output_fd;
	int					last_input_fd;
	char				*internal_pwd;
	char				*internal_oldpwd;
	int					flag;
	struct s_env_cpy	*next;
	struct s_env_cpy	*previous;
}						t_env_cpy;

typedef struct s_command
{
	char				**argv;
	t_token				*token_list;
	char				*infile;
	char				*outfile;
	int					append;
}						t_command;

typedef struct s_redirection_params
{
	char				**input;
	char				**token;
	t_token				**token_list;
	t_env_cpy			*env;
	int					*error_flag;
	char				*quote_type;
}						t_redirection_params;

typedef struct s_var_expansion
{
	char				*result;
	char				*var_name;
	int					var_len;
	int					*i;
}						t_var_expansion;

typedef struct s_pipeline
{
	int         num_commands;
	int         num_pipes;
	int         **pipes;
	pid_t       *pids;
	t_command   **commands;
	t_env_cpy   *env_cpy;
}   t_pipeline;


/* Global Variable */
extern volatile sig_atomic_t	g_last_signal;

/* Execution */
char		**allocate_arguments(t_token *token);
void		execute_command(char *cmd_path, char **av,
			char **env, t_env_cpy *env_cpy);
char		*get_command_path(char **av, t_env_cpy *env_cpy);
void		handle_parent_process(int pid, t_env_cpy *env_cpy);
void		do_comand(t_token *token, t_env_cpy *env_cpy);

/* Redirection */
void		check_redirections(t_token **token, t_env_cpy *env);
void		ft_redirection(t_token **token, t_env_cpy *env);
int			checking_for_errors(t_env_cpy *env, int *in, int *out);
int			check_token(t_token *head);
int			its_redirection(t_token *token);
void		error_occured(int *out, int *in, t_env_cpy *env);
void		dup2_error(int *out, int *in);
char		*get_filename(t_token *token);
int			search_for_redirection(t_token *head);
void		remove_redirection_tokens(t_token **head);
void		remove_red_loop(t_token **current, t_token **prev, t_token **head);
void		ft_infile(t_token *token, t_env_cpy *env);
void		ft_append(t_token *token, t_env_cpy *env);
void		ft_trunck(t_token *token, t_env_cpy *env);

/* Tokenize */
void		add_token(t_token **head, char *input, t_env_cpy *env, char qoute);
char		*extract_quoted_token(char **input, char quote_type);
void		tokenize_input(char *input, t_token **token_list,
				t_env_cpy *env, int *flag);
t_env_cpy	*cpy_env(char **env);
t_env_cpy	*update_env(t_env_cpy *env);

/* Tokenize Check */
int			check_type(char *token, t_env_cpy *env, char quote);
int			if_error(int flag, t_token *token, char *input, t_env_cpy *env_cpy);
void		check(char *input, t_env_cpy *env_cpy);
void		if_input_is_qoutaton(t_input *prepare, char **input);
void		proprocess_loop(t_input *prepare, char **input);
char		*preprocess_input(char *input);
int			is_invalid_pipe_syntax(t_token *token_list);
int			fix_pipe(char *str, t_env_cpy *env);
int			is_build_ins(char *token);
int			pipe_or_redirection(char *token);
int			search_for_pipe(t_token *token_list);
void		error_occurd_status(char *msg, int x, t_env_cpy *env);
void		write_error(char *msg);
t_input		init_input(char *input);
void		check_main_token(t_token **token, t_env_cpy *env_cpy);

/* Tokenize Tools */
int			handle_quote(char **input, char **token, char *quote_type);
int			handle_redirection_token(t_redirection_params *params);
void		handle_special_chars(t_redirection_params *redir_params, int *i);
void		handle_quotes_and_expansion(char **input, char **token,
				t_env_cpy *env, char *quote_type);
void		process_token(char **input, t_token **token_list,
				t_env_cpy *env, int *error_flag);
char		*return_type(char *env);
char		*return_path(char *env);

/* Command Handling */
char		*find_in_path(char *cmd, t_env_cpy *env);
char		*get_old_path(t_env_cpy *env_cpy, char *msg);
int			fill_and_shape_path(char **path_env, char ***paths, t_env_cpy *env);
char		*find_in_path(char *cmd, t_env_cpy *env);
void		ft_exit(t_token *token, t_env_cpy *env);
void		ft_cmd(t_token *token, t_env_cpy *env_cpy, int is_main_shell);

/* Built-in Echo */
void		check_echo(t_token *token, t_env_cpy *env_list);

/* Unset */
void		ft_unset(t_token *token, t_env_cpy **env_cpy);
void		remove_env(char *type, t_env_cpy **env_cpy);

/* Export */
void		ft_export(t_token *token, t_env_cpy *env_cpy);
void		print_sorted(t_env_cpy *head);
void		print_export(t_env_cpy *env_cpy);
t_env_cpy	*a_env(t_env_cpy **head, char *type, char *env, bool equal);

void		add_env_node(t_env_cpy **head, t_env_cpy *new_node);
t_env_cpy	*create_env_node(char *type, char *env, bool equal);
t_env_cpy	*if_equal_new_node(t_env_cpy *new_node, char *env);
t_env_cpy	*fill_else_new_node(t_env_cpy *new_node, bool equal);
void		update_nd(t_env_cpy *node, t_env_cpy *head, char *env, bool equal);
char		*test_dollar(char *env, t_env_cpy *head);
char		*remove_dollar(char *env);
int			find_dollar(char *env);
t_env_cpy	*find_env_node(t_env_cpy *head, char *type);
t_export	init_export(char *env, char *type, bool equal);
void		free_export(t_export export, int flag);

/* Environment */
void		ft_env(t_token *token, t_env_cpy *env_cpy);

/* PWD and CD */
int			dot(t_token **token);
int			old_pwd_is_null(t_cd cd, t_env_cpy *env_cpy);
t_cd		init_cd(t_token *token, t_env_cpy *env_cpy);
t_env_cpy	*add_env_pwd(t_env_cpy *env);
t_env_cpy	*update_env_var(t_env_cpy *env_cpy, char *key, char *value);
char		*get_env_msg(t_env_cpy *tmp, char *msg);
char		*get_cd_path(t_token *token, t_env_cpy *env_cpy, int *should_free);
char		*if_cd_with_dash(t_env_cpy *env_cpy, int *should_free);
char		*if_just_cd(t_env_cpy *env_cpy, int *should_free);
char		*get_cd_path_helper(t_token **token);
char		*get_pwd(t_env_cpy *env_cpy);
char		*get_oldpwd(t_env_cpy *env_cpy);
int			check_after_pwd(t_token *head);
void		if_pwd_avialable(t_env_cpy *env, char *pwd);
void		error_mes_after_checking(t_token *token, t_env_cpy *env);
void		free_at_the_end(t_cd cd);
void		too_long_error_cd(void);
void		freeing_cd(t_cd cd, t_env_cpy *env_cpy, int x);
void		ft_pwd(t_token *token, t_env_cpy *env);
void		ft_cd(t_token *token, t_env_cpy *env_cpy);

/* Utilities */
char		*ft_strndup(const char *s, size_t n);
void		ft_free_2darray(char **tokens);
void		free_token_list(t_token *head);
void		free_env_list(t_env_cpy *head);
char		*ft_strjoin_free(char *s1, char *s2);

/* Environment and Export Tools */
t_env_cpy	*fill_token(t_env_cpy *env_cpy, char *str);
char		*return_type(char *env);
char		*return_path(char *env);
t_env_cpy	*add_shell(t_env_cpy *env_cpy);
char		**list_to_2d(t_env_cpy *env);
void		else_in_env_loop(t_env_cpy **current, t_env_cpy **new_node);
void		check_head(t_env_cpy **head, t_env_cpy **new_node);
void		check_x(int	*x);
bool		check_for_equal(char *env);
void		error_happend(void);
t_env_cpy	*cpy_env_helper(char *env);
void		fill_new_node(t_env_cpy **new_node, t_env_cpy **head);

/* Dollar Expansion */
char		*append_char(char *result, char c);
char		*get_env_value(char *var_name, t_env_cpy *env_list);
char		*handle_special_expansion(char *result, int *i,
				t_env_cpy *env_list, int is_uid);
char		*handle_invalid_variable_expansion(char *result,
				char *var_name, int *i);
char		*handle_digit_variable_expansion(char *result,
				char *var_name, int *i);
char		*expand_variable(char *token, int *i,
				t_env_cpy *env_list, char *result);
char		*expand_token_if_variable(char *token, t_env_cpy *env_list);

/* Signals */
void		setup_signal_handlers(void);
void		handle_sigint(int sig);

/* Redirection Handling */
int			check_token(t_token *head);
void		handle_redirection(char **input, t_token **token_list,
				t_env_cpy *env, int *error_flag);
void		handle_heredoc(char **input, t_env_cpy *env, int *error_flag);

/* Pipe and Command Execution */
char		**allocate_arguments(t_token *token);
char		*get_command_path(char **av, t_env_cpy *env_cpy);
void		execute_command(char *cmd_path, char **av,
				char **env, t_env_cpy *env_cpy);
void		pipe_commands(t_token *token, t_env_cpy *env_cpy);
int			containe_pipe(t_token *token1);

/* Pipeline */
t_command	**parse_commands(t_token *token_list,
				int *num_commands, t_env_cpy *env_cpy);
void		execute_child_process(t_pipeline *pl, int i);

void		execute_pipeline(t_token *token_list, t_env_cpy *env_cpy);

/* Pipe Utils */
void		close_pipes(int **pipes, int num_pipes);
void		free_pipes(int **pipes, int num_pipes);
void		free_commands(t_command **commands);
int			setup_pipeline(t_pipeline *pl, t_token *token_list);
int			**create_pipes(int num_pipes);
void		cleanup_pipeline(t_pipeline *pl);

void		write_error(char *msg);

#endif
