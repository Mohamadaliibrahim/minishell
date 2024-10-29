/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mustafa-machlouch <mustafa-machlouch@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 11:10:38 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/10/29 11:05:23 by mustafa-mac      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

static	int	heredoc_expand_special_variable(char **input,
	t_result_buffer *res_buf, t_env_cpy *env)
{
	char	*status_str;

	if (**input == '?')
	{
		status_str = ft_itoa(env->last_exit_status);
		if (!status_str)
			return (-1);
		if (heredoc_append_string(res_buf, status_str) == -1)
		{
			free(status_str);
			return (-1);
		}
		free(status_str);
		(*input)++;
		return (0);
	}
	return (1);
}

static	int	heredoc_expand_regular_variable(char **input,
	t_result_buffer *res_buf, t_env_cpy *env)
{
	char	*var_name;
	char	*var_value;

	if (extract_variable_name(input, &var_name) == -1)
		return (-1);
	if (var_name)
	{
		var_value = get_env_value(var_name, env);
		free(var_name);
		if (var_value)
		{
			if (heredoc_append_string(res_buf, var_value) == -1)
				return (-1);
		}
	}
	else
	{
		if (heredoc_append_char(res_buf, '$') == -1)
			return (-1);
	}
	return (0);
}

static	int	heredoc_expand_variable(char **input,
	t_result_buffer *res_buf, t_env_cpy *env)
{
	int	ret;

	ret = heredoc_expand_special_variable(input, res_buf, env);
	if (ret == 0)
		return (0);
	else if (ret == -1)
		return (-1);
	return (heredoc_expand_regular_variable(input, res_buf, env));
}

static	int	process_heredoc_input(char **input,
	t_result_buffer *res_buf, t_env_cpy *env)
{
	while (**input)
	{
		if (**input == '$')
		{
			(*input)++;
			if (**input == '\0')
				break ;
			if (heredoc_expand_variable(input, res_buf, env) == -1)
			{
				return (-1);
			}
		}
		else
		{
			if (heredoc_append_char(res_buf, **input) == -1)
			{
				return (-1);
			}
			(*input)++;
		}
	}
	return (0);
}

void	expand_heredoc(char **str, t_env_cpy *env)
{
	char			*input;
	size_t			initial_size;
	t_result_buffer	res_buf;

	input = *str;
	initial_size = ft_strlen(input) * 2 + 1;
	res_buf.buffer = malloc(sizeof(char) * initial_size);
	if (!res_buf.buffer)
		return ;
	res_buf.size = initial_size;
	res_buf.index = 0;
	res_buf.buffer[0] = '\0';
	if (process_heredoc_input(&input, &res_buf, env) == -1)
	{
		free(res_buf.buffer);
		return ;
	}
	res_buf.buffer[res_buf.index] = '\0';
	free(*str);
	*str = res_buf.buffer;
}
