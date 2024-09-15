/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mustafa-machlouch <mustafa-machlouch@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:07:23 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/09/15 10:48:41 by mustafa-mac      ###   ########.fr       */
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
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

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

typedef struct s_shell
{
	char	**env;
	int		last_exit_status;
}	t_shell;

/*tokenize*/
void	add_token(t_token **head, char *input);
char	*extract_quoted_token(char **input, char quote_type);
void	tokenize_input(char *input, t_token **token_list);
/*tokenize_check*/
int		check_type(char *token);
void	check(char *input, char **env);
/*tokenize_tools*/
void	process_token(char **input, t_token **token_list);
/*cmd*/
char	*find_in_path(char *cmd);
void	check_cmnd(char *input, t_token *token, char **env);
int 	execute_command(char **args, t_shell *shell);
/*echo*/
void	check_echo(t_token *token, t_shell *shell);
/*env*/
char	*expand_env_var(char *str, t_shell *shell);
/*pwd_cd */
void	ft_pwd(t_token *token);
void	ft_cd(t_token *token);
/*utils*/
char	*ft_strndup(const char *s, size_t n);
void	ft_free_2darray(char **tokens);
void	free_token_list(t_token *head);
void	init_shell(t_shell *shell, char **envp);


#endif