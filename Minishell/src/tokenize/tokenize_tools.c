/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 15:28:55 by mmachlou          #+#    #+#             */
/*   Updated: 2024/09/22 13:05:24 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char *ft_strjoin_free(char *s1, char *s2)
{
    char *new_str;

    if (!s1 || !s2)
        return NULL;
    new_str = ft_strjoin(s1, s2);
    if (!new_str)
        return NULL;
    free(s1);
    return new_str;
}

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


static void	handle_unquoted(char **input, char **token)
{
    char    *start;
    char    *tmp;
    char    *old_token;

    start = *input;
    while (**input && **input != ' '
        && **input != '\''
        && **input != '"'
        && **input != '\\'
        && **input != '<'
        && **input != '>')
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

	(*input)++;
	(*input)++;
	var_name = ft_strdup("");
	while (**input && **input != '"')
	{
		var_name = append_char(var_name, **input);
		(*input)++;
	}
	(*input)++;
	var_value = getenv(var_name);
	free(var_name);
	if (var_value)
		*token = ft_strjoin_free(*token, var_value);
	while (**input && **input != ' ')
	{
        if (**input == '"')
            (*input)++;
		*token = append_char(*token, **input);
		(*input)++;
	}
}
static void handle_dollar_invalid(char **input, char **token)
{
    (*input)++;
    if ((**input >= '0' && **input <= '9'))
    {
        (*input)++;
    }
    while (**input && **input != ' ')
    {
        
        *token = append_char(*token, **input);
        (*input)++;
    }
}

void handle_dollar_question(char **input, char **token, int last_exit_status)
{
    char *status_str;

    // Convert last_exit_status to a string
    status_str = ft_itoa(last_exit_status);

    // Append the status string to the current token
    *token = ft_strjoin_free(*token, status_str);

    // Move the input pointer past the "$?"
    *input += 2;

    free(status_str);
}

void handle_redirection(char **input, t_token **token_list, t_env_cpy *env, int *error_flag)
{
    char *redirection_token = NULL;

    if (**input == '<' || **input == '>')
    {
        char current_char = **input;
        int count = 0;

        // Count consecutive redirection characters
        while (**input == current_char)
        {
            count++;
            (*input)++;
        }

        if (count > 2)
        {
            // Invalid redirection operator (more than two)
            fprintf(stderr, "minishell: syntax error near unexpected token `%.*s'\n", count, *input - count);
            *error_flag = 1;  // Set error flag
            return;
        }
        else if (count == 1)
        {
            // Single-character redirection operator
            redirection_token = ft_strndup(*input - 1, 1);
        }
        else if (count == 2)
        {
            // Double-character redirection operator
            redirection_token = ft_strndup(*input - 2, 2);
        }

        if (redirection_token)
        {
            add_token(token_list, redirection_token, env, 0);
            free(redirection_token);
        }
    }
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
            if (ft_strlen(token) > 0)
            {
                // Add the accumulated token before the redirection operator
                add_token(token_list, token, env, quote_type);
                free(token);
                token = ft_strdup("");
                quote_type = 0; // Reset quote_type
                if (!token)
                    return;
            }
            handle_redirection(input, token_list, env, error_flag);
            if (*error_flag)
            {
                free(token);
                return;
            }
        }
        else if (**input == '\\' && *(*input + 1) == '$')
        {
            // Handle backslash followed by dollar sign
            handle_backslash(input, &token);  
        }
        else if (**input == '\\')
        {
            // Handle backslash
            handle_backslash(input, &token);  
        }
        else if (**input == '$' && *(*input + 1) >= '0' && *(*input + 1) <= '9')  // Handle $ followed by numbers
        {
            handle_dollar_invalid(input, &token);
        }
        else if (**input == '$' && *(*input + 1) == '=')  // Handle $ followed by '='
        {
            handle_dollar_invalid(input, &token);
        }
        else if (**input == '$' && *(*input + 1) == '?')  // Handle $?
        {
            handle_dollar_question(input, &token, env->last_exit_status);
        }
        else if (**input == '"' || **input == '\'')
        {
            if (!handle_quote(input, &token, &quote_type))
            {
                free(token);
                return;
            }
        }
        else
        {
            // Handle unquoted characters
            handle_unquoted(input, &token);
        }
    }
    if (ft_strlen(token) > 0 && *error_flag == 0)
    {
        add_token(token_list, token, env, quote_type);
        quote_type = 0; // Reset quote_type after adding the token
    }
    free(token);
}
