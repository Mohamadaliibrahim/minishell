/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 15:28:55 by mmachlou          #+#    #+#             */
/*   Updated: 2024/10/18 14:53:09 by mohamibr         ###   ########.fr       */
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

void	handle_dollar_inside_quotes(char **input, char **token, t_env_cpy *env)
{
	char	*var_name;
	char	*var_value;

	(*input)++;  // Move past the '$'

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
        else if (**input == '"' || **input == '\'')
        {
            // Handle double quotes (")
            if (**input == '"')
            {
                (*input)++;  // Skip the opening double quote
                while (**input && **input != '"')
                {
                    // Handle variable expansion inside double quotes
                    if (**input == '$')
                        handle_dollar_inside_quotes(input, &token, env);
                    else
                    {
                        token = append_char(token, **input);
                        (*input)++;
                    }
                }
                (*input)++;  // Skip the closing double quote
            }
            // Handle single quotes ('), which prevent variable expansion
            else if (**input == '\'')
            {
                handle_quote(input, &token, &quote_type);
            }
        }
        else if (ft_strncmp(*input, "$\"", 2) == 0)
            handle_quote(input, &token, &quote_type);
        else if (ft_strncmp(*input, "$\'", 2) == 0)
            handle_quote(input, &token, &quote_type);
        else
        {
            token = append_char(token, **input);
            (*input)++;
        }
    }
    if (ft_strlen(token) > 0 && *error_flag == 0)
    {
        add_token(token_list, token, env, quote_type);
        quote_type = 0; // Reset quote_type after adding the token
    }
    free(token);
}
