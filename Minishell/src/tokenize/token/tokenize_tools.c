/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mustafa-machlouch <mustafa-machlouch@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 15:28:55 by mmachlou          #+#    #+#             */
/*   Updated: 2024/10/29 11:05:44 by mustafa-mac      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

int	handle_quote(char **input, char **token, char *quote_type)
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

int	handle_redirection_token(t_redirection_params *params)
{
	if (ft_strlen(*(params->token)) > 0)
	{
		add_token(params->token_list, *(params->token),
			params->env, *(params->quote_type));
		free(*(params->token));
		*(params->token) = ft_strdup("");
		*(params->quote_type) = 0;
		if (!*(params->token))
			return (1);
	}
	handle_redirection(params->input, params->token_list,
		params->env, params->error_flag);
	if (*(params->error_flag))
	{
		free(*(params->token));
		return (1);
	}
	return (0);
}

void	handle_quotes_and_expansion(char **input, char **token,
	t_env_cpy *env, char *quote_type)
{
	int	i;

	(*input)++;
	*quote_type = '"';
	while (**input && **input != '"')
	{
		if (**input == '$')
		{
			i = 0;
			*token = expand_variable(*input, &i, env, *token);
			*input += i;
		}
		else
		{
			*token = append_char(*token, **input);
			(*input)++;
		}
	}
	if (**input == '"')
		(*input)++;
}

void	handle_special_chars(t_redirection_params *redir_params, int *i)
{
	if (**redir_params->input == '$')
	{
		*redir_params->token = expand_variable(*redir_params->input,
				i, redir_params->env, *redir_params->token);
		*redir_params->input += *i;
	}
	else
	{
		*redir_params->token = append_char(*redir_params->token,
				**redir_params->input);
		(*redir_params->input)++;
	}
}
