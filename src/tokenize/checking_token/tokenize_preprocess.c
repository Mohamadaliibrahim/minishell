/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_preprocess.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 15:25:04 by mmachlou          #+#    #+#             */
/*   Updated: 2024/10/26 15:48:02 by mohamibr         ###   ########.fr       */
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
