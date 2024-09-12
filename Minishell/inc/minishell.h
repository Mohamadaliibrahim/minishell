/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:07:23 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/09/12 20:37:32 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
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

typedef struct s_echo
{
	char			*token;
	struct s_echo	*next;
}					t_echo;



typedef struct s_token
{
	char			*tokens;
	int				token_type;
	struct s_token	*next;
	struct s_token	*previous;
}					t_token;

void	tokenize_input(char *input, t_token **token_list);
char	*find_in_path(char *cmd);
void	free_token_list(t_token *head);
int		check_type(char *token);
char	*ft_strndup(const char *s, size_t n);
void	check_echo(char *input, char *start, char *quoted_token);
#endif