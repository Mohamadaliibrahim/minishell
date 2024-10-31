/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mustafa-machlouch <mustafa-machlouch@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 15:28:55 by mmachlou          #+#    #+#             */
/*   Updated: 2024/10/31 13:03:04 by mustafa-mac      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

static void	handle_special_cases(char **input)
{
	if (**input == '"' && *(*input + 1) == '$' && *(*input + 2) == '"')
	{
		write(1, "$", 1);
		(*input) += 2;
	}
	else if (**input == '\\' && *(*input + 1) == '$')
	{
		write(1, "$", 1);
		(*input) += 2;
	}
}

static	void	finalize_token(t_redirection_params *redir_params)
{
	if (*redir_params->error_flag == 0 && (ft_strlen(*redir_params->token) > 0
			|| *redir_params->quote_type != 0))
		add_token(redir_params->token_list,
			*redir_params->token, redir_params->env, *redir_params->quote_type);
	free(*redir_params->token);
}

void	process_token(char **input, t_token **token_list,
	t_env_cpy *env, int *error_flag)
{
	char					*token;
	char					quote_type;
	int						i;
	t_redirection_params	redir_params;

	token = ft_strdup("");
	quote_type = 0;
	i = 0;
	redir_params = (t_redirection_params){input, &token,
		token_list, env, error_flag, &quote_type};
	if (!token)
		return ;
	while (**input && **input != ' ' && *error_flag == 0)
	{
		handle_special_cases(input);
		if ((**input == '<' || **input == '>'))
		{
			if (handle_redirection_token(&redir_params))
				return ;
			break ;
		}
		handle_token_content(&redir_params, &i);
	}
	finalize_token(&redir_params);
}
