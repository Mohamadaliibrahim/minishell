/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mustafa-machlouch <mustafa-machlouch@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 11:10:38 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/09/24 11:12:26 by mustafa-mac      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void handle_heredoc(char **input, t_token **token_list, t_env_cpy *env, int *error_flag)
{
    char *delimiter;
    char *line;
    int heredoc_fd;
    char *heredoc_file = "/tmp/minishell_heredoc.tmp";

    while (**input && **input == ' ')
        (*input)++;
    delimiter = ft_strndup(*input, ft_strlen(*input));
    while (**input && **input != ' ' && **input != '<' && **input != '>')
        (*input)++;
    heredoc_fd = open(heredoc_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (heredoc_fd < 0)
    {
        perror("minishell: heredoc");
        *error_flag = 1;
        free(delimiter);
        return;
    }
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
    add_token(token_list, ft_strdup(heredoc_file), env, HEREDOC);
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
            fprintf(stderr, "minishell: syntax error near unexpected token `%.*s'\n", count, *input - count);
            *error_flag = 1;  // Set error flag
            return;
        }
        else if (count == 2 && current_char == '<')
        {
            handle_heredoc(input, token_list, env, error_flag);
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