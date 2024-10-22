/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 15:28:55 by mmachlou          #+#    #+#             */
/*   Updated: 2024/10/22 11:02:17 by mohamibr         ###   ########.fr       */
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

static void handle_dollar_inside_quotes(char **input, char **token, t_env_cpy *env)
{
    (*input)++;  // Move past the '$'

    // Check if the next character is not a valid variable name
    if (!ft_isalnum(**input) && **input != '_')
    {
        // Append '$' literally since it's not followed by a valid variable
        *token = append_char(*token, '$');
        return;
    }

    char *var_name;
    char *var_value;

    // Handle special case for "$?"
    if (**input == '?')
    {
        (*input)++;  // Move past the '?'
        var_value = ft_itoa(env->last_exit_status);  // Get the last exit status as a string
        *token = ft_strjoin_free(*token, var_value);  // Append the exit status
        free(var_value);
        return;
    }

    // Normal variable handling
    var_name = ft_strdup("");  // Initialize var_name to an empty string

    // Collect the variable name until we hit a non-alphanumeric or non-underscore character
    while (**input && (ft_isalnum(**input) || **input == '_'))
    {
        var_name = append_char(var_name, **input);  // Build the variable name
        (*input)++;
    }

    // Lookup the value of the environment variable
    var_value = get_env_value(var_name, env);
    free(var_name);  // Free the variable name after use

    // If the environment variable is found, append its value to the token
    if (var_value)
        *token = ft_strjoin_free(*token, var_value);
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
        if (**input == '$')
            handle_dollar_inside_quotes(input, token, env);
        else
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
