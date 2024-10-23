/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_preprocess.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 15:25:04 by mmachlou          #+#    #+#             */
/*   Updated: 2024/10/23 15:40:53 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

void	if_input_is_qoutaton(t_input *prepare, char **input)
{
	(*prepare).quote = (*input)[(*prepare).i++];
	(*prepare).new_input[(*prepare).j++] = (*prepare).quote;
	while ((*input)[(*prepare).i] && (*input)[(*prepare).i] != (*prepare).quote)
		(*prepare).new_input[(*prepare).j++] = (*input)[(*prepare).i++];
	if ((*input)[(*prepare).i] == (*prepare).quote)
		(*prepare).new_input[(*prepare).j++] = (*input)[(*prepare).i++];
}

void	proprocess_loop(t_input *prepare, char **input)
{
	if ((*input)[(*prepare).i] == '\'' || (*input)[(*prepare).i] == '"')
		if_input_is_qoutaton(prepare, input);
	else if ((*input)[(*prepare).i] == '|' || (*input)[(*prepare).i] == '<'
		|| (*input)[(*prepare).i] == '>')
	{
		if ((*prepare).i > 0 && (*input)[(*prepare).i - 1] != ' ')
			(*prepare).new_input[(*prepare).j++] = ' ';
		(*prepare).new_input[(*prepare).j++] = (*input)[(*prepare).i];
		if (((*input)[(*prepare).i] == '<' || (*input)[(*prepare).i] == '>')
			&& (*input)[(*prepare).i + 1] == (*input)[(*prepare).i])
		{
			(*prepare).i++;
			(*prepare).new_input[(*prepare).j++] = (*input)[(*prepare).i];
		}
		if ((*input)[(*prepare).i + 1] && (*input)[(*prepare).i + 1] != ' ')
			(*prepare).new_input[(*prepare).j++] = ' ';
		(*prepare).i++;
	}
	else
		(*prepare).new_input[(*prepare).j++] = (*input)[(*prepare).i++];
}

char	*preprocess_input(char *input)
{
	t_input	prepare;

	prepare = init_input(input);
	if (!prepare.new_input)
		return (NULL);
	while (input[prepare.i])
		proprocess_loop(&prepare, &input);
	prepare.new_input[prepare.j] = '\0';
	return (prepare.new_input);
}

int	is_invalid_pipe_syntax(t_token *token_list)
{
	t_token	*current;

	current = token_list;
	if (current->token_type == PIPE)
		return (1);
	while (current->next)
	{
		if (current->token_type == PIPE && current->next->token_type == PIPE)
			return (1);
		current = current->next;
	}
	if (current->token_type == PIPE)
		return (1);
	return (0);
}

int	its(char *str, int i)
{
	if ((str[i + 1] == '"' || str[i + 1] == '\'') && str[i + 2] == '\0')
	{
		write_error("Minishell:""syntax error near unexpected token `|'\n");
		return (1);
	}
	if ((str[i + 1] == '"' || str[i + 1] == '\'')
		&& (str[i + 2] == '"' || str[i + 2] == '\''))
	{
		write_error("Minishell:""syntax error near unexpected token `|'\n");
		return (1);
	}
	return (0);
}

int	fix_pipe(char *str)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			if (quote == 0)
				quote = str[i];
			else if (quote == str[i])
				quote = 0;
			// If different quote, stay in current quote
		}
		else if (str[i] == '|' && quote == 0)
		{
			if (str[i + 1] == '|')
			{
				write_error("Minishell: syntax error near unexpected token `||'\n");
				return (1);
			}
			if (str[i + 1] == '\0' || (str[i + 1] == ' ' && str[i + 2] == '\0'))
			{
				write_error("Minishell: syntax error near unexpected token `|'\n");
				return (1);
			}
			if (its(str, i))
				return (1);
		}
		i++;
	}
	return (0);
}

//stop!!