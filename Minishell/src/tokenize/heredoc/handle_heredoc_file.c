/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc_file.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mustafa-machlouch <mustafa-machlouch@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 11:10:38 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/10/29 11:05:27 by mustafa-mac      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

static	int	open_heredoc_file(char *heredoc_file)
{
	int	heredoc_fd;

	heredoc_fd = open(heredoc_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (heredoc_fd < 0)
	{
		perror("minishell: heredoc");
		return (-1);
	}
	return (heredoc_fd);
}

static	ssize_t	read_line_from_stdin(char *buffer,
	size_t buffer_size, char *delimiter)
{
	ssize_t	bytes_read;

	write(STDOUT_FILENO, "> ", 2);
	bytes_read = read(STDIN_FILENO, buffer, buffer_size - 1);
	if (bytes_read == 0)
	{
		write_error("minishell: warning: here-document ");
		write_error("delimited by end-of-file (wanted `");
		write_error(delimiter);
		write_error("')\n");
		return (0);
	}
	if (bytes_read < 0)
	{
		perror("minishell: read");
		return (-1);
	}
	buffer[bytes_read] = '\0';
	return (bytes_read);
}

static	int	process_line(int heredoc_fd, char *buffer, t_env_cpy *env)
{
	char	*line;

	line = strdup(buffer);
	if (!line)
	{
		return (-1);
	}
	expand_heredoc(&line, env);
	write(heredoc_fd, line, strlen(line));
	free(line);
	return (0);
}

static	int	read_and_process_heredoc_lines(int heredoc_fd,
	char *delimiter, t_env_cpy *env)
{
	char	buffer[BUFFER_SIZE];
	size_t	delimiter_len;
	ssize_t	bytes_read;

	delimiter_len = strlen(delimiter);
	while (1)
	{
		bytes_read = read_line_from_stdin(buffer, BUFFER_SIZE, delimiter);
		if (bytes_read <= 0)
		{
			if (bytes_read == 0)
				return (0);
			else
				return (-1);
		}
		if (check_for_signal() == -1)
			return (-1);
		if (is_line_delimiter(buffer, delimiter, delimiter_len))
			break ;
		if (process_line(heredoc_fd, buffer, env) == -1)
			return (-1);
	}
	return (0);
}

int	handle_heredoc_file(char *heredoc_file, char *delimiter, t_env_cpy *env)
{
	int	heredoc_fd;
	int	result;

	heredoc_fd = open_heredoc_file(heredoc_file);
	if (heredoc_fd < 0)
		return (-1);
	result = read_and_process_heredoc_lines(heredoc_fd, delimiter, env);
	close(heredoc_fd);
	if (result == -1)
	{
		unlink(heredoc_file);
		return (-1);
	}
	return (0);
}
