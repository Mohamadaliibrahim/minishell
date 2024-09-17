/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 15:28:55 by mmachlou          #+#    #+#             */
/*   Updated: 2024/09/17 23:44:15 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"


static void	handle_backslash(char **input, char **token)
{
	char	*tmp;
	char	*old_token;

	(*input)++;
	if (**input)
	{
		tmp = ft_strndup(*input, 1);
		old_token = *token;
		*token = ft_strjoin(*token, tmp);
		free(old_token);
		free(tmp);
		(*input)++;
	}
}

static int	handle_quote(char **input, char **token)
{
	char	quote_char;
	char	*tmp;
	char	*old_token;

	quote_char = **input;
	tmp = extract_quoted_token(input, quote_char);
	if (tmp)
	{
		old_token = *token;
		*token = ft_strjoin(*token, tmp);
		free(old_token);
		free(tmp);
		return (1);
	}
	else
		return (0);
}

static void	handle_unquoted(char **input, char **token)
{
	char	*start;
	char	*tmp;
	char	*old_token;

	start = *input;
	while (**input && **input != ' '
		&& **input != '\''
		&& **input != '"' && **input != '\\')
		(*input)++;
	tmp = ft_strndup(start, *input - start);
	old_token = *token;
	*token = ft_strjoin(*token, tmp);
	free(old_token);
	free(tmp);
}


void	process_token(char **input, t_token **token_list)
{
	char	*token;

	token = ft_strdup("");
	if (!token)
		return ;
	while (**input && **input != ' ')
	{
		if (**input == '\\')
			handle_backslash(input, &token);
		else if (**input == '\'' || **input == '"')
		{
			if (!handle_quote(input, &token))
			{
				free(token);
				return ;
			}
		}
		else if (ft_strncmp(*input, "$\"", 2) == 0)
			handle_quote(input, &token);
		else
			handle_unquoted(input, &token);
	}
	add_token(token_list, token);
	free(token);
}
