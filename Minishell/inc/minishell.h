/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:07:23 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/10/13 07:43:54 by mohamibr         ###   ########.fr       */
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
	struct s_env_cpy	*next;
	struct s_env_cpy	*previous;
}					t_env_cpy;

typedef struct s_command
{
    char        **argv;      // Command arguments
    t_token     *token_list; // Tokens for this command
    char        *infile;     // Input redirection file (<)
    char        *outfile;    // Output redirection file (>, >>)
    int         append;      // Boolean for append redirection (>>)
}               t_command;

/* Global Variable */
extern volatile	sig_atomic_t g_last_signal;

/* Execution */
void		do_comand(t_token *token, t_env_cpy *env_cpy);

/* Redirection */
void		check_redirections(t_token **token, t_env_cpy *env);
void		ft_redirection(t_token **token, t_env_cpy *env);
int			search_for_redirection(t_token *token1);

/* Tokenize */
void		add_token(t_token **head, char *input, t_env_cpy *env, char qoute);
char		*extract_quoted_token(char **input, char quote_type);
void		tokenize_input(char *input, t_token **token_list, t_env_cpy *env, int *flag);
t_env_cpy	*cpy_env(char **env);
t_env_cpy	*update_env(t_env_cpy *env);

/* Tokenize Check */
int			check_type(char *token, t_env_cpy *env);
void		check(char *input, t_env_cpy *env_cpy);

/* Tokenize Tools */
void		process_token(char **input, t_token **token_list, t_env_cpy *env, int *error_flag);
char		*return_type(char *env);
char		*return_path(char *env);

/* Command Handling */
char		*find_in_path(char *cmd, t_env_cpy *env);
char		*get_old_path(t_env_cpy *env_cpy, char *msg);
void		ft_cmd(t_token *token, t_env_cpy *env_cpy, int is_main_shell);

/* Built-in Echo */
void		check_echo(t_token *token, t_env_cpy *env_list);

/* Unset */
void		ft_unset(t_token *token, t_env_cpy **env_cpy);
void		remove_env(char *type, t_env_cpy **env_cpy);
void		free_single_env_list(t_env_cpy *head);

/* expand */
void		expand(t_token *head, t_env_cpy *env);
char		*find(char *str, t_env_cpy *head);

/* Export */
void		ft_export(t_token *token, t_env_cpy *env_cpy);
void		print_sorted(t_env_cpy *head);
void		print_export(t_env_cpy *env_cpy);
t_env_cpy	*a_env(t_env_cpy **head, char *type, char *env, bool equal);
void		update_pwd_oldpwd(t_env_cpy *env, char *new_pwd, char *old_pwd);

/* Environment */
void		ft_env(t_token *token, t_env_cpy *env_cpy);

/* PWD and CD */
void		ft_pwd(t_token *token, t_env_cpy *env);
void		ft_cd(t_token *token, t_env_cpy *env_cpy);
t_env_cpy	*update_env_var(t_env_cpy *env_cpy, char *key, char *value);

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
bool		check_for_equal(char *env);
char		**list_to_2d(t_env_cpy *env);
t_env_cpy	*cpy_env_helper(char *env);

/* Dollar Expansion */
char		*append_char(char *result, char c);
char		*expand_token_if_variable(char *token, t_env_cpy *env_list);
char		*get_env_value(char *var_name, t_env_cpy *env_list);

/* Signals */
void		setup_signal_handlers(void);
void		handle_sigint(int sig);
void 		heredoc_sigint_handler(int signo);

/* Redirection Handling */
int			check_token(t_token *head);
void 		handle_redirection(char **input, t_token **token_list, t_env_cpy *env, int *error_flag);
void		handle_heredoc(char **input, t_env_cpy *env, int *error_flag);

/* Pipe and Command Execution */
char 		**allocate_arguments(t_token *token);
char 		*get_command_path(char **av, t_env_cpy *env_cpy);
void 		execute_command(char *cmd_path, char **av, char **env, t_env_cpy *env_cpy);
void 		pipe_commands(t_token *token, t_env_cpy *env_cpy);
int			containe_pipe(t_token *token1);

/* Pipeline */
t_command	**parse_commands(t_token *token_list, int *num_commands, t_env_cpy *env_cpy);
void 		execute_pipeline(t_token *token_list, t_env_cpy *env_cpy);

/* Pipe Utils */
int 		**create_pipes(int num_pipes);
void 		close_pipes(int **pipes, int num_pipes);
void 		free_pipes(int **pipes, int num_pipes);
void 		free_commands(t_command **commands);

/*Handle Redirections in Pipe*/
int 		search_for_redirection_input(t_token *token_list);
int 		search_for_redirection_output(t_token *token_list);
int 		handle_input_redirection(t_token *token);
int 		handle_output_redirection(t_token *token, int append);
void 		free_pids_and_commands(pid_t *pids, t_command **commands);

#endif
