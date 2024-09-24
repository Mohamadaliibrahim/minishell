/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mustafa-machlouch <mustafa-machlouch@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:07:23 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/09/24 15:41:20 by mustafa-mac      ###   ########.fr       */
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

typedef enum e_token_type
{
	CMND,
	PIPE,
	REDIRECT_IN,
	REDIRECT_OUT,
	APPEND,
	HEREDOC,
	VARIABLE,
	QUOTE,
	DQUOTE,
	UNKNOWN
}					t_token_type;

typedef struct s_token
{
	char			*tokens;
	int				token_type;
	char			qoute_type;
	struct s_token	*next;
	struct s_token	*previous;
}					t_token;

typedef struct s_env_cpy
{
	char				*env;
	char				*type;
	char 				*heredoc_file;
	bool				equal;
	int					last_exit_status;
	int					last_output_fd;
	int					last_input_fd;
	struct s_env_cpy	*next;
	struct s_env_cpy	*previous;
}					t_env_cpy;

/*Global valriable*/
extern volatile	sig_atomic_t g_last_signal;

/*execution*/
void		do_comand(t_token *token, t_env_cpy *env_cpy);
/*redirection*/
void		check_redirections(t_token *token, t_env_cpy *env);
void		ft_redirection(t_token *token, t_env_cpy *env);
int			search_for_redirection(t_token *token1);
/*tokenize*/
void		add_token(t_token **head, char *input, t_env_cpy *env, char qoute);
char		*extract_quoted_token(char **input, char quote_type);
void		tokenize_input(char *input, t_token **token_list, t_env_cpy *env, int *flag);
t_env_cpy	*cpy_env(char **env);
t_env_cpy	*update_env(t_env_cpy *env);
/*tokenize_check*/
int			check_type(char *token, t_env_cpy *env);
void		check(char *input, t_env_cpy *env_cpy);
/*tokenize_tools*/
void		process_token(char **input, t_token **token_list, t_env_cpy *env, int *error_flag);
char		*return_type(char *env);
char		*return_path(char *env);
/*cmd*/
char		*find_in_path(char *cmd, t_env_cpy *env);
char		*get_old_path(t_env_cpy *env_cpy, char *msg);
void		ft_cmd(t_token *token, t_env_cpy *env_cpy);
/*echo*/
void		check_echo(t_token *token, t_env_cpy *env_list);
/*unset*/
void		ft_unset(t_token *token, t_env_cpy *env_cpy);
void		free_single_env_list(t_env_cpy *head);
/*export*/
void		ft_export(t_token *token, t_env_cpy *env_cpy);
void		print_sorted(t_env_cpy *head);
void		print_export(t_env_cpy *env_cpy);
t_env_cpy	*a_env(t_env_cpy **head, char *type, char *env, bool equal);
void		update_pwd_oldpwd(t_env_cpy *env, char *new_pwd, char *old_pwd);
/*env*/
void		ft_env(t_token *token, t_env_cpy *env_cpy);
/*pwd_cd */
void		ft_pwd(void);
void		ft_cd(t_token *token, t_env_cpy *env_cpy);
/*utils*/
char		*ft_strndup(const char *s, size_t n);
void		ft_free_2darray(char **tokens);
void		free_token_list(t_token *head);
void		free_env_list(t_env_cpy *head);
char 		*ft_strjoin_free(char *s1, char *s2);
void		ft_free_2darray(char **tokens);
/*Env && Export*/
char		*return_type(char *env);
char		*return_path(char *env);
t_env_cpy	*add_shell(t_env_cpy *env_cpy);
bool		check_for_equal(char *env);
char		**list_to_2d(t_env_cpy *env);
t_env_cpy	*cpy_env_helper(char *env);
/*DOllar $*/
char		*append_char(char *result, char c);
char		*expand_token_if_variable(char *token, t_env_cpy *env_list);
/*Signals*/
void		setup_signal_handlers(void);
void		handle_sigint(int sig);
/*Redirections */
void 	handle_redirection(char **input, t_token **token_list, t_env_cpy *env, int *error_flag);
void	handle_heredoc(char **input, t_env_cpy *env, int *error_flag);

#endif