/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc_tools.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mustafa-machlouch <mustafa-machlouch@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 11:10:38 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/10/29 11:05:31 by mustafa-mac      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

int	heredoc_append_char(t_result_buffer *res_buf, char c)
{
	char	*new_buffer;

	if (res_buf->index + 1 >= res_buf->size)
	{
		res_buf->size *= 2;
		new_buffer = realloc(res_buf->buffer, res_buf->size);
		if (!new_buffer)
			return (-1);
		res_buf->buffer = new_buffer;
	}
	res_buf->buffer[res_buf->index++] = c;
	res_buf->buffer[res_buf->index] = '\0';
	return (0);
}

int	heredoc_append_string(t_result_buffer *res_buf, const char *str)
{
	char	*new_buffer;
	size_t	len;

	len = ft_strlen(str);
	if (res_buf->index + len >= res_buf->size)
	{
		while (res_buf->index + len >= res_buf->size)
			res_buf->size *= 2;
		new_buffer = realloc(res_buf->buffer, res_buf->size);
		if (!new_buffer)
			return (-1);
		res_buf->buffer = new_buffer;
	}
	strcpy(res_buf->buffer + res_buf->index, str);
	res_buf->index += len;
	return (0);
}

int	extract_variable_name(char **input, char **var_name)
{
	char	*var_start;
	size_t	var_len;

	var_start = *input;
	while (**input && (ft_isalnum(**input) || **input == '_'))
		(*input)++;
	var_len = *input - var_start;
	if (var_len > 0)
	{
		*var_name = ft_substr(var_start, 0, var_len);
		if (!*var_name)
			return (-1);
	}
	else
	{
		*var_name = NULL;
	}
	return (0);
}

int	check_for_signal(void)
{
	if (g_last_signal == SIGINT)
	{
		g_last_signal = 0;
		return (-1);
	}
	return (0);
}

int	is_line_delimiter(char *buffer, char *delimiter, size_t delimiter_len)
{
	if (strncmp(buffer, delimiter, delimiter_len) == 0
		&& buffer[delimiter_len] == '\n')
		return (1);
	else
		return (0);
}
