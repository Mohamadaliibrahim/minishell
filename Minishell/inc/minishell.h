/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:07:23 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/09/20 16:08:52 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>
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
	struct s_token	*next;
	struct s_token	*previous;
}					t_token;

typedef struct s_env_cpy
{
	char				*env;
	char				*type;
	bool				equal;
	int					last_exit_status;
	struct s_env_cpy	*next;
	struct s_env_cpy	*previous;
}					t_env_cpy;

/*Global valriable*/
// extern	volatile	sig_atomic_t g_shell_status;

/*tokenize*/
void		add_token(t_token **head, char *input, t_env_cpy *env);
char		*extract_quoted_token(char **input, char quote_type);
void		tokenize_input(char *input, t_token **token_list, t_env_cpy *env);
t_env_cpy	*cpy_env(char **env);
t_env_cpy	*update_env(t_env_cpy *env);
/*tokenize_check*/
int			check_type(char *token, t_env_cpy *env);
void		check(char *input, t_env_cpy *env_cpy);
/*tokenize_tools*/
void		process_token(char **input, t_token **token_list, t_env_cpy *env);
char		*return_type(char *env);
char		*return_path(char *env);
/*cmd*/
char		*find_in_path(char *cmd, t_env_cpy *env);
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
char		*return_type(char *env);
char		*return_path(char *env);
bool		check_for_equal(char *env);
char		*expand_token_if_variable(char *token, t_env_cpy *env_list);
char		*append_char(char *result, char c);
void		setup_signal_handlers(void);
void		ft_free_2darray(char **tokens);
char		**list_to_2d(t_env_cpy *env);
char		*get_old_path(t_env_cpy *env_cpy, char *msg);
t_env_cpy	*cpy_env_helper(char *env);
t_env_cpy	*add_shell(t_env_cpy *env_cpy);

#endif