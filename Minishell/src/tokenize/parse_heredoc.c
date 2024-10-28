/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mustafa-machlouch <mustafa-machlouch@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 11:10:38 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/10/28 14:33:00 by mustafa-mac      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static	void	skip_spaces(char **input)
{
	while (**input && **input == ' ')
		(*input)++;
}

static	char	set_quote_char(char **input)
{
	char	qc;

	qc = '\0';
	if (**input == '"' || **input == '\'')
	{
		qc = **input;
		(*input)++;
	}
	return (qc);
}

static	size_t	calculate_delimiter_length(char *input, char qc)
{
	size_t	delimiter_length;

	delimiter_length = 0;
	while (input[delimiter_length])
	{
		if (qc != '\0' && input[delimiter_length] == qc)
			break ;
		else if (qc == '\0' && strchr(" <>", input[delimiter_length]) != NULL)
			break ;
		delimiter_length++;
	}
	return (delimiter_length);
}

static	void	set_delimiter(char **input, char **delimiter,
	size_t delimiter_length, int *error_flag)
{
	if (delimiter_length == 0)
	{
		write_error("minishell: syntax error near ");
		write_error("unexpected token `newline'\n");
		*error_flag = 1;
		return ;
	}
	*delimiter = ft_strndup(*input, delimiter_length);
	*input += delimiter_length;
}

void	parse_heredoc_delimiter(char **input, char **delimiter,
	int *error_flag, char *quote_char)
{
	size_t	delimiter_length;

	skip_spaces(input);
	*quote_char = set_quote_char(input);
	delimiter_length = calculate_delimiter_length(*input, *quote_char);
	set_delimiter(input, delimiter, delimiter_length, error_flag);
	if (*quote_char != '\0' && delimiter_length > 0)
		(*input)++;
}
