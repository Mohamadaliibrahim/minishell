/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokonize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:07:00 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/09/13 22:59:33 by mohamibr         ###   ########.fr       */
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

char	*ft_strndup(const char *s, size_t n)
{
	char	*dup;

	dup = malloc(n + 1);
	if (!dup)
		return (NULL);
	ft_memcpy(dup, s, n);
	dup[n] = '\0';
	return (dup);
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
		token = ft_strndup(start, end - start);
		*input = end + 1;
		return (token);
	}
	return (NULL);
}

void	tokenize_input(char *input, t_token **token_list)
{
	while (*input)
	{
		while (*input == ' ')
			input++;
		if (*input)
			process_token(&input, token_list);
	}
}

void	process_token(char **input, t_token **token_list)
{
	char	*token;
	char	*tmp;
	char	quote_char;
	char	*start;
	char	*old_token;

	token = ft_strdup("");
	if (!token)
		return ;
	while (**input && **input != ' ')
	{
		if (**input == '\\')
		{
			(*input)++;
			if (**input)
			{
				tmp = ft_strndup(*input, 1);
				old_token = token;
				token = ft_strjoin(token, tmp);
				free(old_token);
				free(tmp);
				(*input)++;
			}
		}
		else if (**input == '\'' || **input == '"')
		{
			quote_char = **input;
			tmp = extract_quoted_token(input, quote_char);
			if (tmp)
			{
				old_token = token;
				token = ft_strjoin(token, tmp);
				free(old_token);
				free(tmp);
			}
			else
			{
				// Handle error: unmatched quote
				free(token);
				return ;
			}
		}
		else
		{
			start = *input;
			while (**input && **input != ' ' && **input != '\'' && **input != '"' && **input != '\\')
				(*input)++;
			tmp = ft_strndup(start, *input - start);
			old_token = token;
			token = ft_strjoin(token, tmp);
			free(old_token);
			free(tmp);
		}
	}
	add_token(token_list, token);
	free(token);
}
