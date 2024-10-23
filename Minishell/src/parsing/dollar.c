/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 14:24:44 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/10/20 12:45:10 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// Append normal characters to the result
char	*append_char(char *result, char c)
{
	char	temp_str[2];
	char	*temp;

	temp_str[0] = c;
	temp_str[1] = '\0';
	temp = ft_strjoin(result, temp_str);
	free(result);
	return (temp);
}

// Retrieve environment value
char	*get_env_value(char *var_name, t_env_cpy *env_list)
{
	t_env_cpy	*current;

	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->type, var_name) == 0)
			return (current->env);
		current = current->next;
	}
	return (NULL);
}

// Retrieve user ID (UID) as a special case
char *handle_uid_expansion(char *result, int *i)
{
    char *uid_str;
    char *temp;

    uid_str = ft_itoa(getuid());  // Retrieve the user ID using system call
    temp = ft_strjoin(result, uid_str);
    free(uid_str);
    free(result);
    *i += 4;  // Skip $ and 'UID'
    return temp;
}

// Handle $? expansion (last command exit status)
char *handle_exit_status_expansion(char *result, int *i, t_env_cpy *env_list)
{
    char *exit_status_str;
    char *temp;

    exit_status_str = ft_itoa(env_list->last_exit_status);
    temp = ft_strjoin(result, exit_status_str);
    free(exit_status_str);
    free(result);
    *i += 2;  // Skip $ and ?
    return temp;
}

char *handle_invalid_variable_expansion(char *result, char *var_name, int *i)
{
    char *temp;

    // Append the '$' to the result
    temp = ft_strjoin(result, "$");
    free(result);

    // If there is a character after the '$', append it
    if (var_name[0] != '\0')
    {
        result = ft_strjoin(temp, (char[]){var_name[0], '\0'});
        free(temp);
        // Increment index by 2 to skip the '$' and the invalid character
        *i += 2;
    }
    else
    {
        // If there's nothing after '$', just increment index by 1
        result = temp;
        *i += 1;
    }

    return result;
}


// Handle cases where variable starts with a digit (e.g., $9HOME)
char *handle_digit_variable_expansion(char *result, char *var_name, int *i)
{
    char *temp;
    char *substr;
    int var_len = 0;

    var_name += 1;  // Skip the digit after $
    *i += 2;  // Skip $ and the digit

    while (var_name[var_len] && (ft_isalnum(var_name[var_len]) || var_name[var_len] == '_'))
        var_len++;

    substr = ft_substr(var_name, 0, var_len);
    if (substr)
    {
        temp = ft_strjoin(result, substr);
        free(substr);
        free(result);
        result = temp;
    }
    *i += var_len;
    return result;
}

// Handle general variable expansion (normal alphanumeric variables)
char *handle_general_variable_expansion(char *result, char *var_name, int var_len, int *i, t_env_cpy *env_list)
{
    char *substr;
    char *env_value;
    char *temp;

    substr = ft_substr(var_name, 0, var_len);
    env_value = get_env_value(substr, env_list);
    free(substr);

    if (!env_value)
        env_value = ft_strdup("");  // Handle cases where the env variable is not found
    else
        env_value = ft_strdup(env_value);  // Create a copy to avoid modifying the original string

    temp = ft_strjoin(result, env_value);
    free(result);
    free(env_value);

    *i += var_len + 1;
    return temp;
}

// Main expand variable function, delegates to specialized functions
char *expand_variable(char *token, int *i, t_env_cpy *env_list, char *result)
{
    char *var_name = token + (*i) + 1;
    int var_len;

    // Handle $'HO''ME' case: skip the dollar sign, process quoted content
    if (var_name[0] == '\'')
    {
        (*i)++;  // Move past the $
        while (token[*i])  // Process content inside quotes
        {
            if (token[*i] != '\'')  // Only append if the character is not a single quote
            {
                result = append_char(result, token[*i]);
            }
            (*i)++;  // Always increment the index to move to the next character
        }
        return result;
    }
    // Handle $"HOME" case: skip the dollar sign and quotes, return the inner content
    if (var_name[0] == '"')
    {
        (*i)++; // Move past the initial dollar sign and the opening quote
        while (token[*i] && token[*i] != '"')  // Collect everything inside the quotes
        {
            result = append_char(result, token[*i]);
            (*i)++;
        }
        if (token[*i] == '"')  // Skip the closing quote
            (*i)++;
        return result;
    }

    // Handle $UID special case
    if (ft_strncmp(var_name, "UID", 3) == 0)
        return handle_uid_expansion(result, i);

    // Handle $? special case
    if (var_name[0] == '?')
        return handle_exit_status_expansion(result, i, env_list);

    // Handle non-alphanumeric or '_' after $
    if (!ft_isalnum(var_name[0]) && var_name[0] != '_')
        return handle_invalid_variable_expansion(result, var_name, i);

    // Handle cases where the variable starts with a digit
    if (ft_isdigit(var_name[0]))
        return handle_digit_variable_expansion(result, var_name, i);

    // Calculate the length of the variable name (alphanumeric and '_')
    var_len = 0;
    while (ft_isalnum(var_name[var_len]) || var_name[var_len] == '_')
        var_len++;

    // Handle general variable expansion
    return handle_general_variable_expansion(result, var_name, var_len, i, env_list);
}

// Main function to expand tokens, handling escape characters
char	*expand_token_if_variable(char *token, t_env_cpy *env_list)
{
	char	*result;
	int		i;

	i = 0;
	result = ft_strdup("");
	while (token[i])
	{
		if (token[i] == '$' && token[i + 1] == '\\')
		{
			result = append_char(result, '$');  // Append $ literally
			i += 2;  // Skip both '\' and '$'
		}      
		else if (token[i] == '$')
		{
			result = expand_variable(token, &i, env_list, result);
		}
		else
		{
			// Append normal characters
			result = append_char(result, token[i]);
			i++;
		}
	}
	return (result);
}
