/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 11:10:38 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/10/23 15:15:36 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static sigjmp_buf heredoc_jmpbuf;

void heredoc_sigint_handler(int signo)
{
    (void)signo;  // Suppress unused parameter warning
    write(STDOUT_FILENO, "\n", 1);  // Output ^C followed by a newline
    siglongjmp(heredoc_jmpbuf, 1);  // Jump to the set jump point
}


static void parse_heredoc_delimiter(char **input, char **delimiter, int *error_flag)
{
    char quote_char = '\0';
    size_t delimiter_length = 0;

    while (**input && **input == ' ')
        (*input)++;
    if (**input == '"' || **input == '\'')
    {
        quote_char = **input;
        (*input)++;
    }
    while ((*input)[delimiter_length])
    {
        if (quote_char != '\0' && (*input)[delimiter_length] == quote_char)
            break;
        else if (quote_char == '\0' && strchr(" <>", (*input)[delimiter_length]) != NULL)
            break;
        delimiter_length++;
    }
    if (delimiter_length == 0)
    {
        write_error("minishell: syntax error near unexpected token `newline'\n");
        *error_flag = 1;
        return;
    }
    *delimiter = ft_strndup(*input, delimiter_length);
    *input += (quote_char != '\0') ? delimiter_length + 1 : delimiter_length;
}

static int handle_heredoc_file(char *heredoc_file, char *delimiter)
{
    int heredoc_fd = open(heredoc_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    char * volatile line = NULL;  // Declare line as volatile pointer to char
    struct sigaction sa, old_sa;

    if (heredoc_fd < 0)
    {
        perror("minishell: heredoc");
        return (-1);
    }

    // Set up custom SIGINT handler
    sa.sa_handler = heredoc_sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;  // Not using SA_RESTART
    sigaction(SIGINT, &sa, &old_sa);

    // Set jump point
    if (sigsetjmp(heredoc_jmpbuf, 1) == 0)
    {
        while (1)
        {
            line = readline("> ");
            if (!line)
            {
                write_error("Minishell: warning: here-document delimited by end-of-file (wanted `");
                write_error(delimiter);
                write_error("')\n");
                break;
            }
            if (ft_strcmp(line, delimiter) == 0)
            {
                free(line);
                break;
            }
            write(heredoc_fd, line, ft_strlen(line));
            write(heredoc_fd, "\n", 1);
            free(line);
        }
    }
    else
    {
        close(heredoc_fd);
        unlink(heredoc_file);
        // Restore original signal handler
        sigaction(SIGINT, &old_sa, NULL);
        return (-1);  // Indicate that heredoc was interrupted
    }

    // Restore original signal handler
    sigaction(SIGINT, &old_sa, NULL);
    close(heredoc_fd);
    return (0);
}

void handle_heredoc(char **input, t_env_cpy *env, int *error_flag)
{
    char *delimiter;
    int heredoc_result;
    char *heredoc_file = "/tmp/minishell_heredoc.tmp";

    parse_heredoc_delimiter(input, &delimiter, error_flag);
    if (*error_flag)
        return;
    heredoc_result = handle_heredoc_file(heredoc_file, delimiter);
    if (heredoc_result < 0)
    {
        free(delimiter);
        *error_flag = 1;
        return;
    }
    free(delimiter);
    env->heredoc_file = ft_strdup(heredoc_file);
}

void handle_redirection(char **input, t_token **token_list, t_env_cpy *env, int *error_flag)
{
    char *redirection_token = NULL;

    if (**input == '<' || **input == '>')
    {
        char current_char = **input;
        int count = 0;
        while (**input == current_char)
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
            return;
        }
        else if (count == 2 && current_char == '<')
        {
            // Handle heredoc separately without adding it to the token list
            handle_heredoc(input, env, error_flag);
            return;
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
