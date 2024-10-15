/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 15:28:55 by mmachlou          #+#    #+#             */
/*   Updated: 2024/10/13 13:24:16 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void handle_dollar_inside_quotes(char **input, char **token, t_env_cpy *env_list, int last_exit_status)
{
    char *var_value;
    char *var_name;

    (*input)++;  // Skip the initial '$'

    // Special case for $?
    if (**input == '?')
    {
        // Convert last_exit_status to a string and append it to the token
        char *status_str = ft_itoa(last_exit_status);
        *token = ft_strjoin_free(*token, status_str);  // Append the exit status to the token
        free(status_str);
        (*input)++;  // Move past '?'
        return;
    }

    // Extract the variable name
    var_name = ft_strdup("");
    while (**input && (ft_isalnum(**input) || **input == '_'))  // Accept alphanumeric and underscores for variable names
    {
        var_name = append_char(var_name, **input);
        (*input)++;
    }

    // Expand the variable using the env_list
    var_value = get_env_value(var_name, env_list);
    if (var_value)
    {
        *token = ft_strjoin_free(*token, var_value);  // Append the expanded variable value to the token
    }
    free(var_name);

    // Continue parsing the remaining characters inside the double quotes
    while (**input && **input != '"')
    {
        *token = append_char(*token, **input);  // Append other characters inside the quotes
        (*input)++;
    }

    // Skip the closing double quote
    if (**input == '"')
        (*input)++;
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

static void handle_unquoted(char **input, char **token, t_env_cpy *env)
{
    while (**input && **input != ' '
        && **input != '\''
        && **input != '"'
        && **input != '\\'
        && **input != '<'
        && **input != '>')
    {
        if (**input == '$')
        {
            (*input)++;
            if (**input == '?')
            {
                // Handle $?
                char *status_str = ft_itoa(env->last_exit_status);
                *token = ft_strjoin_free(*token, status_str);
                free(status_str);
                (*input)++;
            }
            else
            {
                char *var_name = ft_strdup("");
                while (**input && (ft_isalnum(**input) || **input == '_'))
                {
                    var_name = append_char(var_name, **input);
                    (*input)++;
                }
                char *var_value = get_env_value(var_name, env);
                if (var_value)
                {
                    *token = ft_strjoin_free(*token, var_value);
                }
                else
                {
                    // If variable not found, replace with empty string
                    *token = ft_strjoin_free(*token, "");
                }
                free(var_name);
            }
        }
        else
        {
            *token = append_char(*token, **input);
            (*input)++;
        }
    }
}


void handle_special_cases(char **input, char **token, int last_exit_status)
{
    if (**input == '\\')
    {
        if (**input)
        {
            *token = append_char(*token, **input);
            (*input)++;
        }
    }
    else if (**input == '$' && *(*input + 1) == '?')
    {
        // Handle $?: Replace with last exit status
        char *status_str = ft_itoa(last_exit_status);
        *token = ft_strjoin_free(*token, status_str);
        *input += 2;  // Skip over the `$?`
        free(status_str);
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
        else if (**input == '\\' || (**input == '$' && *(*input + 1) == '?'))
            handle_special_cases(input, &token, env->last_exit_status);
        else if (**input == '"' || **input == '\'')
        {
            if (**input == '"')
            {
                // Handle double-quote
                (*input)++;  // Skip the opening quote
                while (**input && **input != '"')
                {
                    if (**input == '$' && *(*input + 1) == '?')
                        handle_dollar_inside_quotes(input, &token, env, env->last_exit_status);  // Handle $?
                    else
                        token = append_char(token, **input);
                    (*input)++;
                }
                if (**input == '"')
                    (*input)++;  // Skip the closing quote
            }
            else if (!handle_quote(input, &token, &quote_type))
            {
                free(token);
                return;
            }
        }
        else if (ft_strncmp(*input, "$\"", 2) == 0)
            handle_quote(input, &token, &quote_type);
        else if (ft_strncmp(*input, "$\'", 2) == 0)
            handle_quote(input, &token, &quote_type);
        else
            handle_unquoted(input, &token, env);
    }
    if (ft_strlen(token) > 0 && *error_flag == 0)
    {
        add_token(token_list, token, env, quote_type);
        quote_type = 0; // Reset quote_type after adding the token
    }
    free(token);
}
