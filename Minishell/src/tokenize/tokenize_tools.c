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
\
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
            if (**input == '"' && *(*input + 1) == '$' && *(*input + 2) != '\0')
                handle_dollar_inside_quotes(input, &token);
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
