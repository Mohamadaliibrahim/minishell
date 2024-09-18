/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mustafa-machlouch <mustafa-machlouch@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 15:28:55 by mmachlou          #+#    #+#             */
/*   Updated: 2024/09/18 16:03:26 by mustafa-mac      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// Join two strings and free the first one
char *ft_strjoin_free(char *s1, char *s2)
{
    char *new_str;

    if (!s1 || !s2)
        return NULL;
    new_str = ft_strjoin(s1, s2); // Use your existing ft_strjoin function
    if (!new_str)
        return NULL;
    free(s1);  // Free the old string
    return new_str;
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

static void handle_backslash(char **input, char **token)
{
    (*input)++;  // Skip the backslash
    if (**input == '$')  // If it's a literal dollar sign, treat it as part of the token
    {
        (*input)++;  // Move past the dollar sign
        while (**input && **input != ' ')  // Append characters following the literal $ (e.g., HOME)
        {
            *token = append_char(*token, **input);
            (*input)++;
        }
    }
    else if (**input)  // Handle other escaped characters
    {
        *token = append_char(*token, **input);  // Append the escaped character
        (*input)++;
    }
}

void	handle_dollar_inside_quotes(char **input, char **token)
{
	char	*var_name;
	char	*var_value;

	(*input)++;  // Skip the initial "
	(*input)++;  // Skip the $
	var_name = ft_strdup("");
	while (**input && **input != '"')
	{
		var_name = append_char(var_name, **input);
		(*input)++;
	}
	(*input)++;  // Skip closing quote
	var_value = getenv(var_name);
	free(var_name);
	if (var_value)
		*token = ft_strjoin_free(*token, var_value);
	while (**input && **input != ' ')
	{
		*token = append_char(*token, **input);
		(*input)++;
	}
}
static void handle_dollar_invalid(char **input, char **token)
{
    (*input)++;  // Skip the dollar sign

    // Check if the next character is a digit (invalid variable name)
    if (**input >= '0' && **input <= '9')
    {
        (*input)++;  // Skip the invalid variable (e.g., $9)
    }

    // Append the rest of the string (e.g., "HOME") as a literal
    while (**input && **input != ' ')
    {
        *token = append_char(*token, **input);
        (*input)++;
    }
}

void process_token(char **input, t_token **token_list)
{
    char *token;

    token = ft_strdup("");
    if (!token)
        return;
    while (**input && **input != ' ')
    {
        if (**input == '\\')
            handle_backslash(input, &token);  // This ensures that \$HOME is handled correctly
        else if (**input == '$' && *(*input + 1) >= '0' && *(*input + 1) <= '9')  // Handle $ followed by numbers
            handle_dollar_invalid(input, &token);  // Skip invalid variables like $9 and append the rest
        else if (**input == '"' && *(*input + 1) == '$' && *(*input + 2) != '\0') // Handle "$HO"ME case
            handle_dollar_inside_quotes(input, &token);

        else if (**input == '\'' || **input == '"')
        {
            if (!handle_quote(input, &token))
            {
                free(token);
                return;
            }
        }
        else if (ft_strncmp(*input, "$\"", 2) == 0)
            handle_quote(input, &token);
        else if (ft_strncmp(*input, "$\'", 2) == 0)
            handle_quote(input, &token);
        else
            handle_unquoted(input, &token);
    }
    add_token(token_list, token);
    free(token);
}
