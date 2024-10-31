/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_token_content.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mustafa-machlouch <mustafa-machlouch@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 15:28:55 by mmachlou          #+#    #+#             */
/*   Updated: 2024/10/31 13:16:26 by mustafa-mac      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

static	int	is_valid_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

static	void	handle_quotes(char **input, char **token,
					t_env_cpy *env, char *quote_type)
{
	if (**input == '"')
		handle_quotes_and_expansion(input, token, env, quote_type);
	else if (**input == '\'')
		handle_quote(input, token, quote_type);
}

static	void	handle_dollar_sign(t_redirection_params *redir_params, int *i)
{
	char	*status_str;

	if (*(*redir_params->input + 1) == '?')
	{
		status_str = ft_itoa(redir_params->env->last_exit_status);
		*redir_params->token = ft_strjoin_free
			(*redir_params->token, status_str);
		(*redir_params->input) += 2;
		free(status_str);
	}
	else if (!is_valid_var_char(*(*redir_params->input + 1)))
	{
		*redir_params->token = append_char(*redir_params->token, '$');
		(*redir_params->input)++;
	}
	else
		handle_special_chars(redir_params, i);
}

void	handle_token_content(t_redirection_params *redir_params, int *i)
{
	if (**redir_params->input == '"' || **redir_params->input == '\'')
		handle_quotes(redir_params->input, redir_params->token,
			redir_params->env, redir_params->quote_type);
	else if (**redir_params->input == '$')
		handle_dollar_sign(redir_params, i);
	else
		handle_special_chars(redir_params, i);
}
