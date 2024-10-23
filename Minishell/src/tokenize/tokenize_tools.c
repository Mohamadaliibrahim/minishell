/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 15:28:55 by mmachlou          #+#    #+#             */
/*   Updated: 2024/10/23 15:15:17 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	handle_quote(char **input, char **token, char *quote_type)
{
	char	q_char;
	char	*tmp;
	char	*old_token;

	q_char = **input;
	tmp = extract_quoted_token(input, q_char);
	if (tmp)
	{
		old_token = *token;
		*token = ft_strjoin(*token, tmp);
		free(old_token);
		free(tmp);
		*quote_type = q_char;
		return (1);
	}
	else
		return (0);
}

// Handle redirection logic
static int handle_redirection_token(char **input, char **token, t_token **token_list, t_env_cpy *env, int *error_flag, char *quote_type)
{
    if (ft_strlen(*token) > 0)
    {
        add_token(token_list, *token, env, *quote_type);
        free(*token);
        *token = ft_strdup("");
        *quote_type = 0;
        if (!*token)
            return (1);
    }
    handle_redirection(input, token_list, env, error_flag);
    if (*error_flag)
    {
        free(*token);
        return (1);
    }
    return (0);
}

// Handle quotes and variable expansion inside quotes
static void handle_quotes_and_expansion(char **input, char **token, t_env_cpy *env, char *quote_type)
{
    (*input)++;  // Skip the opening double quote
    *quote_type = '"';  // Set quote_type to double quote
    while (**input && **input != '"')
    {
        if (**input == '$')  // Handle variable expansion inside quotes
        {
            int i = 0; // Initialize index for variable expansion
            *token = expand_variable(*input, &i, env, *token);
            *input += i; // Advance the input pointer by the number of characters consumed
        }
        else  // Append regular characters inside quotes
        {
            *token = append_char(*token, **input);
            (*input)++;
        }
    }
    if (**input == '"')  // Ensure we are at the closing quote
        (*input)++;  // Skip the closing double quote
}

void process_token(char **input, t_token **token_list, t_env_cpy *env, int *error_flag)
{
    char *token;
    char quote_type = 0;

    token = ft_strdup("");
    if (!token)
        return;

    while (**input && **input != ' ' && *error_flag == 0)
    {
        if ((**input == '<' || **input == '>'))
        {
            if (handle_redirection_token(input, &token, token_list, env, error_flag, &quote_type))
                return;
            break ;
        }
        else if (**input == '"' && *(*input + 1) == '$' && *(*input + 2) == '"')  // Skip backslash and print the dollar sign
        {
            write(1, "$", 1);  // Directly print the $ character without appending it to the token
            (*input) += 2;  // Skip both the backslash and the dollar sign
        }
        else if (**input == '\\' && *(*input + 1) == '$')  // Skip backslash and print the dollar sign
        {
            write(1, "$", 1);  // Directly print the $ character without appending it to the token
            (*input) += 2;  // Skip both the backslash and the dollar sign
        }
        else if (**input == '"')
            handle_quotes_and_expansion(input, &token, env, &quote_type);
        else if (**input == '\'')
            handle_quote(input, &token, &quote_type);
        else if (**input == '$')
        {
            int i = 0; // Initialize index
            token = expand_variable(*input, &i, env, token);
            *input += i; // Advance the input pointer by the number of characters consumed
        }
        else
        {
            token = append_char(token, **input);
            (*input)++;
        }
    }
    if (*error_flag == 0 && (ft_strlen(token) > 0 || quote_type != 0))
        add_token(token_list, token, env, quote_type);
    free(token);
}
