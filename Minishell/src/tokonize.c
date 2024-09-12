/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokonize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:07:00 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/09/12 11:07:42 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static t_token	*new_token(char *input)
{
	t_token	*new_node;

	new_node = malloc(sizeof(t_token));
	if (!new_node)
		return (NULL);
	new_node->tokens = ft_strdup(input);
	new_node->next = NULL;
	new_node->token_type = check_type(input);
	new_node->previous = NULL;
	return (new_node);
}

static void	add_token(t_token **head, char *input)
{
	t_token	*new_node;
	t_token	*tmp;

	new_node = new_token(input);
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

static char	*extract_quoted_token(char **input, char quote_type)
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
		token = strndup(start, end - start);
		*input = end + 1;
		return (token);
	}
	return (NULL);
}
void	tokenize_input(char *input, t_token **token_list)
{
	char	*quote_token;
	char	*start;

	while (*input)
	{
		while (*input == ' ')
			input++;
		if (*input == '\'' || *input == '"')
		{
			quote_token = extract_quoted_token(&input, *input);
			if (quote_token)
				add_token(token_list, quote_token);
		}
		else if (*input)
		{
			start = input;
			while (*input && *input != ' ' && *input != '\'' && *input != '"')
				input++;
			add_token(token_list, strndup(start, input - start));
		}
	}
}
