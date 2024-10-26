/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 11:10:38 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/10/26 19:59:22 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static sigjmp_buf heredoc_jmpbuf;

void heredoc_sigint_handler(int signo)
{
    (void)signo;  // Suppress unused parameter warning
    write(STDOUT_FILENO, "\n", 1);  // Output newline
    siglongjmp(heredoc_jmpbuf, 1);  // Jump to the set jump point
}

static void parse_heredoc_delimiter(char **input, char **delimiter, int *error_flag, char *quote_char)
{
    char qc = '\0';
    size_t delimiter_length = 0;

    while (**input && **input == ' ')
        (*input)++;
    if (**input == '"' || **input == '\'')
    {
        qc = **input;
        (*input)++;
    }
    while ((*input)[delimiter_length])
    {
        if (qc != '\0' && (*input)[delimiter_length] == qc)
            break;
        else if (qc == '\0' && strchr(" <>", (*input)[delimiter_length]) != NULL)
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
    *input += (qc != '\0') ? delimiter_length + 1 : delimiter_length;
    *quote_char = qc;
}

int append_to_result(char **dest, size_t *dest_size, size_t *dest_len, const char *src)
{
    size_t src_len = ft_strlen(src);
    // Check if buffer needs to be resized
    while (*dest_len + src_len + 1 > *dest_size)
    {
        size_t new_size = *dest_size * 2;
        char *new_dest = realloc(*dest, new_size);
        if (!new_dest)
            return (-1);
        *dest = new_dest;
        *dest_size = new_size;
    }
    // Append src to dest
    ft_strcpy(*dest + *dest_len, src);
    *dest_len += src_len;
    return (0);
}

void expand_heredoc(char **str, t_env_cpy *env)
{
    char *input = *str;
    size_t buffer_size = ft_strlen(input) * 2 + 1; // Initial buffer size
    char *result = malloc(sizeof(char) * buffer_size);
    if (!result)
        return;
    result[0] = '\0'; // Initialize as empty string
    size_t j = 0; // Current length of result

    while (*input)
    {
        if (*input == '$')
        {
            input++;
            if (*input == '\0')
                break;
            // Handle special case for "$?"
            if (*input == '?')
            {
                char *status_str = ft_itoa(env->last_exit_status);
                if (status_str)
                {
                    if (append_to_result(&result, &buffer_size, &j, status_str) == -1)
                    {
                        free(result);
                        return;
                    }
                    free(status_str);
                }
                input++;
                continue;
            }
            // Extract variable name
            char *var_start = (char *)input;
            while (*input && (ft_isalnum(*input) || *input == '_'))
                input++;
            size_t var_len = input - var_start;
            if (var_len > 0)
            {
                char *var_name = ft_substr(var_start, 0, var_len);
                if (var_name)
                {
                    char *var_value = get_env_value(var_name, env);
                    if (var_value)
                    {
                        if (append_to_result(&result, &buffer_size, &j, var_value) == -1)
                        {
                            free(var_name);
                            free(result);
                            return;
                        }
                    }
                    free(var_name);
                }
            }
            else
            {
                // If there's no valid variable name, treat '$' as literal
                char dollar = '$';
                if (append_to_result(&result, &buffer_size, &j, &dollar) == -1)
                {
                    free(result);
                    return;
                }
            }
        }
        else
        {
            char c = *input;
            char s[2] = {c, '\0'};
            if (append_to_result(&result, &buffer_size, &j, s) == -1)
            {
                free(result);
                return;
            }
            input++;
        }
    }
    result[j] = '\0';
    free(*str);
    *str = result;
}


static int handle_heredoc_file(char *heredoc_file, char *delimiter, t_env_cpy *env, char quote_char)
{
    int heredoc_fd = open(heredoc_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    char *line = NULL;  // Removed 'volatile' qualifier
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
            if (quote_char == '\0')
                expand_heredoc(&line, env); // Pass address to modify the line
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
    char quote_char = '\0'; // Initialize quote_char
    int heredoc_result;
    char *heredoc_file = "/tmp/minishell_heredoc.tmp";

    parse_heredoc_delimiter(input, &delimiter, error_flag, &quote_char);
    if (*error_flag)
        return;
    heredoc_result = handle_heredoc_file(heredoc_file, delimiter, env, quote_char);
    if (heredoc_result < 0)
    {
        free(delimiter);
        *error_flag = 1;
        return;
    }
    free(delimiter);
    if (env->heredoc_file != NULL)
        free(env->heredoc_file);
    env->heredoc_file = ft_strdup(heredoc_file);
    if (!env->heredoc_file)
    {
        write_error("minishell: Memory allocation failed for heredoc file\n");
        *error_flag = 1;
    }
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
