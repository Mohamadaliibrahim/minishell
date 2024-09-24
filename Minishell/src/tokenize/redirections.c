/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 11:10:38 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/09/24 13:21:13 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	handle_heredoc(char **input, t_token **token_list, t_env_cpy *env,
		int *error_flag)
{
	char	*delimiter;
	char	*line;
	int		heredoc_fd;
	char	*heredoc_file;

	heredoc_file = "/tmp/minishell_heredoc.tmp";
	// Skip spaces after '<<'
	while (**input && **input == ' ')
		(*input)++;
	// Extract delimiter until the next space or redirection symbol
	size_t delimiter_length = strcspn(*input, " <>");
	if (delimiter_length == 0)
	{
		fprintf(stderr, "minishell: syntax error near unexpected token `newline'\n");
		*error_flag = 1;
		return ;
	}
	delimiter = ft_strndup(*input, delimiter_length);
	*input += delimiter_length;
	// Open the temporary file to write heredoc content
	heredoc_fd = open(heredoc_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (heredoc_fd < 0)
	{
		perror("minishell: heredoc");
		*error_flag = 1;
		free(delimiter);
		return ;
	}
	// Read lines until the delimiter is encountered
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(heredoc_fd, line, ft_strlen(line));
		write(heredoc_fd, "\n", 1);
		free(line);
	}
	// Close the file and free resources
	close(heredoc_fd);
	free(delimiter);
	// Add the heredoc file path as a token
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
            *error_flag = 1;
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
