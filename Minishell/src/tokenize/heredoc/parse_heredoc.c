/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mustafa-machlouch <mustafa-machlouch@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 11:10:38 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/10/30 12:26:13 by mustafa-mac      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

void	parse_quoted_part(char **input, char **delimiter, int *error_flag)
{
	char	quote_char;
	char	to_append[2];

	quote_char = **input;
	(*input)++;
	while (**input && **input != quote_char)
	{
		to_append[0] = **input;
		to_append[1] = '\0';
		*delimiter = ft_strjoin_free(*delimiter, to_append);
		if (!*delimiter)
		{
			*error_flag = 1;
			return ;
		}
		(*input)++;
	}
	if (**input == quote_char)
		(*input)++;
}

void	parse_delimiter(char **input, char **delimiter, int *error_flag)
{
	char	to_append[2];

	while (**input)
	{
		if (**input == '"' || **input == '\'')
		{
			parse_quoted_part(input, delimiter, error_flag);
			if (*error_flag)
				return ;
		}
		else if (strchr(" <>", **input) != NULL)
			break ;
		else
		{
			to_append[0] = **input;
			to_append[1] = '\0';
			*delimiter = ft_strjoin_free(*delimiter, to_append);
			if (!*delimiter)
			{
				*error_flag = 1;
				return ;
			}
			(*input)++;
		}
	}
}

void	parse_heredoc_delimiter(char **input, char **delimiter,
			int *error_flag)
{
	*delimiter = ft_strdup("");
	if (!*delimiter)
	{
		*error_flag = 1;
		return ;
	}
	if (*error_flag)
		return ;
	while (**input && **input == ' ')
		(*input)++;
	parse_delimiter(input, delimiter, error_flag);
	if (*error_flag)
		return ;
	if (ft_strlen(*delimiter) == 0)
	{
		write_error("minishell: syntax error near ");
		write_error("unexpected token `newline'\n");
		*error_flag = 1;
	}
}
