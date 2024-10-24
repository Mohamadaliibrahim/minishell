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

int	handle_redirection_token(char **input, char **token,
	t_token **token_list, t_token_context *ctx)
{
	if (ft_strlen(*token) > 0)
	{
		add_token(token_list, *token, ctx->env, *(ctx->quote_type));
		free(*token);
		*token = ft_strdup("");
		*(ctx->quote_type) = 0;
		if (!*token)
			return (1);
	}
	handle_redirection(input, token_list, ctx->env, ctx->error_flag);
	if (*(ctx->error_flag))
	{
		free(*token);
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

void	handle_special_cases(char **input)
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
