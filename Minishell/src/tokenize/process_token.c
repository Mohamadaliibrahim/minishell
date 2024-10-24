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

static	char	*initialize_token_context(t_token_context *ctx,
	t_env_cpy *env, int *error_flag, char *quote_type)
{
	char	*token;

	token = ft_strdup("");
	if (!token)
		return (NULL);
	ctx->env = env;
	ctx->error_flag = error_flag;
	ctx->quote_type = quote_type;
	return (token);
}

static	void	handle_regular_characters(char **input,
	char **token, t_env_cpy *env)
{
	int	i;

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

static void	handle_token_processing(char **input, char **token,
	t_token **token_list, t_token_context *ctx)
{
	if ((**input == '<' || **input == '>'))
	{
		if (handle_redirection_token(input, token, token_list, ctx))
			return ;
	}
	handle_special_cases(input);
	if (**input == '"')
		handle_quotes_and_expansion(input, token, ctx->env, ctx->quote_type);
	else if (**input == '\'')
		handle_quote(input, token, ctx->quote_type);
	else
		handle_regular_characters(input, token, ctx->env);
}

void	process_token(char **input, t_token **token_list,
	t_env_cpy *env, int *error_flag)
{
	char			*token;
	char			quote_type;
	t_token_context	ctx;

	quote_type = 0;
	token = initialize_token_context(&ctx, env, error_flag, &quote_type);
	if (!token)
		return ;
	while (**input && **input != ' ' && *error_flag == 0)
	{
		handle_token_processing(input, &token, token_list, &ctx);
	}
	if (*error_flag == 0 && (ft_strlen(token) > 0 || quote_type != 0))
		add_token(token_list, token, env, quote_type);
	free(token);
}
