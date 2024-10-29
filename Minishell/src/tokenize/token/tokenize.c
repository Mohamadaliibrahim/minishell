/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mustafa-machlouch <mustafa-machlouch@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:07:00 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/10/29 11:05:50 by mustafa-mac      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

static t_token	*new_token(char *input, t_env_cpy *env, char qoute_type)
{
	t_token	*new_node;

	new_node = malloc(sizeof(t_token));
	if (!new_node)
		return (NULL);
	new_node->tokens = ft_strdup(input);
	if (!new_node->tokens)
	{
		free(new_node);
		return (NULL);
	}
	new_node->next = NULL;
	new_node->token_type = check_type(input, env, qoute_type);
	new_node->qoute_type = qoute_type;
	new_node->previous = NULL;
	return (new_node);
}

void	add_token(t_token **head, char *input, t_env_cpy *env, char qoute)
{
	t_token	*new_node;
	t_token	*tmp;

	new_node = new_token(input, env, qoute);
	if (!new_node)
		return ;
	if (!*head)
		*head = new_node;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_node;
		new_node->previous = tmp;
	}
}

char	*extract_quoted_token(char **input, char quote_type)
{
	char	*end;
	char	*start;
	char	*token;

	end = ++(*input);
	start = end;
	while (*end && *end != quote_type)
		end++;
	if (*end == quote_type)
	{
		token = ft_strndup(start, end - start);
		*input = end + 1;
		return (token);
	}
	return (NULL);
}

void	tokenize_input(char *input, t_token **token_list, t_env_cpy *env,
		int *flag)
{
	while (*input)
	{
		while (*input == ' ')
			input++;
		if (*input)
			process_token(&input, token_list, env, flag);
		if (*flag)
			break ;
	}
}
