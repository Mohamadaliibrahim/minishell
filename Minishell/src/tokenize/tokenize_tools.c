/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mustafa-machlouch <mustafa-machlouch@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 15:28:55 by mmachlou          #+#    #+#             */
/*   Updated: 2024/09/23 16:03:36 by mustafa-mac      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char *ft_strjoin_free(char *s1, char *s2)
{
    char *new_str;

    if (!s1 || !s2)
        return NULL;
    new_str = ft_strjoin(s1, s2);
    if (!new_str)
        return NULL;
    free(s1);
    return new_str;
}

static int	handle_quote(char **input, char **token, char *quote_type)
{
	char	q_char;
	char	*tmp;
	char	*old_token;

	q_char = **input;
	tmp = extract_quoted_token(input, q_char);
	if (tmp)
	{
		old_token = *token;
		*token = ft_strjoin(*token, tmp);
		free(old_token);
		free(tmp);
		*quote_type = q_char;
		return (1);
	}
	else
		return (0);
}


static void	handle_unquoted(char **input, char **token)
{
    char    *start;
    char    *tmp;
    char    *old_token;

    start = *input;
    while (**input && **input != ' '
        && **input != '\''
        && **input != '"'
        && **input != '\\'
        && **input != '<'
        && **input != '>')
        (*input)++;
    tmp = ft_strndup(start, *input - start);
    old_token = *token;
    *token = ft_strjoin(*token, tmp);
    free(old_token);
    free(tmp);
}


static void handle_backslash(char **input, char **token)
{
    while (**input && **input != ' ')  // Append characters following the literal $ (e.g., HOME)
    {
        *token = append_char(*token, **input);
        (*input)++;
    }
}

void	handle_dollar_inside_quotes(char **input, char **token)
{
	char	*var_name;
	char	*var_value;

	(*input)++;
	(*input)++;
	var_name = ft_strdup("");
	while (**input && **input != '"')
	{
		var_name = append_char(var_name, **input);
		(*input)++;
	}
	(*input)++;
	var_value = getenv(var_name);
	free(var_name);
	if (var_value)
		*token = ft_strjoin_free(*token, var_value);
	while (**input && **input != ' ')
	{
        if (**input == '"')
            (*input)++;
		*token = append_char(*token, **input);
		(*input)++;
	}
}

void handle_dollar_question(char **input, char **token, int last_exit_status)
{
    char *status_str;

    status_str = ft_itoa(last_exit_status);
    *token = ft_strjoin_free(*token, status_str);
    *input += 2;

    free(status_str);
}

void handle_heredoc(char **input, t_token **token_list, t_env_cpy *env, int *error_flag)
{
    char *delimiter;
    char *line;
    int heredoc_fd;
    char *heredoc_file = "/tmp/minishell_heredoc.tmp";

    // Extract the delimiter following the `<<` operator
    while (**input && **input == ' ')
        (*input)++;
    delimiter = ft_strndup(*input, ft_strlen(*input));

    // Open a temporary file to store heredoc content
    heredoc_fd = open(heredoc_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (heredoc_fd < 0)
    {
        perror("minishell: heredoc");
        *error_flag = 1;
        free(delimiter);
        return;
    }

    // Read lines from the user until the delimiter is encountered
    while (1)
    {
        line = readline("> ");
        if (!line || ft_strcmp(line, delimiter) == 0)
            break;
        write(heredoc_fd, line, ft_strlen(line));
        write(heredoc_fd, "\n", 1);
        free(line);
    }
    close(heredoc_fd);
    free(line);
    free(delimiter);

    // Replace the heredoc token with the temporary file name
    add_token(token_list, ft_strdup(heredoc_file), env, 0);
}


void handle_redirection(char **input, t_token **token_list, t_env_cpy *env, int *error_flag)
{
    char *redirection_token = NULL;

    if (**input == '<' || **input == '>')
    {
        char current_char = **input;
        int count = 0;

        // Count consecutive redirection characters
        while (**input == current_char)
        {
            count++;
            (*input)++;
        }

        if (count > 2)
        {
            // Invalid redirection operator (more than two)
            fprintf(stderr, "minishell: syntax error near unexpected token `%.*s'\n", count, *input - count);
            *error_flag = 1;  // Set error flag
            return;
        }
        else if (count == 2 && current_char == '<')
        {
            // Handle heredoc `<<`
            handle_heredoc(input, token_list, env, error_flag);
            return;
        }
        else if (count == 1)
        {
            // Single-character redirection operator
            redirection_token = ft_strndup(*input - 1, 1);
        }
        else if (count == 2)
        {
            // Double-character redirection operator
            redirection_token = ft_strndup(*input - 2, 2);
        }

        if (redirection_token)
        {
            add_token(token_list, redirection_token, env, 0);
            free(redirection_token);
        }
    }
}


void process_token(char **input, t_token **token_list, t_env_cpy *env, int *error_flag)
{
    char *token;
    char quote_type = 0;

    token = ft_strdup("");
    if (!token)
        return;
    while (**input && **input != ' ' && *error_flag == 0)
    {
        if ((**input == '<' || **input == '>'))
        {
            if (ft_strlen(token) > 0)
            {
                // Add the accumulated token before the redirection operator
                add_token(token_list, token, env, quote_type);
                free(token);
                token = ft_strdup("");
                quote_type = 0; // Reset quote_type
                if (!token)
                    return;
            }
            handle_redirection(input, token_list, env, error_flag);
            if (*error_flag)
            {
                free(token);
                return;
            }
        }
        else if (**input == '\\')
            handle_backslash(input, &token);  
        else if (**input == '$' && *(*input + 1) == '?')  // Handle $?
            handle_dollar_question(input, &token, env->last_exit_status);
        else if (**input == '"' || **input == '\'')
        {
            if (**input == '"' && *(*input + 1) == '$' && *(*input + 2) != '\0')
                handle_dollar_inside_quotes(input, &token);
            else if (!handle_quote(input, &token, &quote_type))
            {
                free(token);
                return;
            }
        }
        else if (ft_strncmp(*input, "$\"", 2) == 0)
            handle_quote(input, &token, &quote_type);
        else if (ft_strncmp(*input, "$\'", 2) == 0)
            handle_quote(input, &token, &quote_type);
        else
            handle_unquoted(input, &token);
    }
    if (ft_strlen(token) > 0 && *error_flag == 0)
    {
        add_token(token_list, token, env, quote_type);
        quote_type = 0; // Reset quote_type after adding the token
    }
    free(token);
}