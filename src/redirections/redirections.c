/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mustafa-machlouch <mustafa-machlouch@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 11:10:38 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/10/30 12:03:03 by mustafa-mac      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	append_to_result(char **dest, size_t *dest_size,
	size_t *dest_len, const char *src)
{
	size_t	src_len;
	size_t	new_size;
	char	*new_dest;

	src_len = ft_strlen(src);
	while (*dest_len + src_len + 1 > *dest_size)
	{
		new_size = *dest_size * 2;
		new_dest = realloc(*dest, new_size);
		if (!new_dest)
			return (-1);
		*dest = new_dest;
		*dest_size = new_size;
	}
	ft_strcpy(*dest + *dest_len, src);
	*dest_len += src_len;
	return (0);
}

static	void	process_heredoc(char *delimiter,
	t_env_cpy *env, int *error_flag)
{
	int		heredoc_result;
	char	*heredoc_file;

	heredoc_file = "/tmp/minishell_heredoc.tmp";
	heredoc_result = handle_heredoc_file(heredoc_file, delimiter, env);
	if (heredoc_result < 0)
	{
		*error_flag = 1;
		return ;
	}
	if (env->heredoc_file != NULL)
		free(env->heredoc_file);
	env->heredoc_file = ft_strdup(heredoc_file);
	if (!env->heredoc_file)
	{
		write_error("minishell: Memory allocation failed for heredoc file\n");
		*error_flag = 1;
	}
}

void	handle_heredoc(char **input, t_env_cpy *env, int *error_flag)
{
	char	*delimiter;

	parse_heredoc_delimiter(input, &delimiter, error_flag);
	if (*error_flag)
		return ;
	process_heredoc(delimiter, env, error_flag);
	free(delimiter);
}

static	int	parse_redirection(char **input, int *error_flag, char *current_char)
{
	int	count;

	count = 0;
	*current_char = **input;
	while (**input == *current_char)
	{
		count++;
		(*input)++;
	}
	if (count > 2)
	{
		write_error("minishell: syntax error near unexpected token `");
		write(STDERR_FILENO, *input - count, count);
		write_error("'\n");
		*error_flag = 1;
		return (-1);
	}
	return (count);
}

void	handle_redirection(char **input, t_token **token_list,
	t_env_cpy *env, int *error_flag)
{
	char	*redirection_token;
	char	current_char;
	int		count;

	redirection_token = NULL;
	if (**input == '<' || **input == '>')
	{
		count = parse_redirection(input, error_flag, &current_char);
		if (*error_flag)
			return ;
		if (count == 2 && current_char == '<')
		{
			handle_heredoc(input, env, error_flag);
			return ;
		}
		else if (count == 1)
			redirection_token = ft_strndup(*input - 1, 1);
		else if (count == 2)
			redirection_token = ft_strndup(*input - 2, 2);
		if (redirection_token)
		{
			add_token(token_list, redirection_token, env, 0);
			free(redirection_token);
		}
	}
}
