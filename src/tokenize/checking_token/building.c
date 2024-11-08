/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   building.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 08:57:24 by mohamibr          #+#    #+#             */
/*   Updated: 2024/10/23 17:13:58 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

t_input	init_input(char *input)
{
	t_input	prepare;

	prepare.i = 0;
	prepare.j = 0;
	prepare.quote = '\0';
	prepare.len = ft_strlen(input);
	prepare.new_len = prepare.len * 2;
	prepare.new_input = malloc(prepare.new_len + 1);
	if (!prepare.new_input)
		prepare.new_input = NULL;
	return (prepare);
}

static int	check_for_quotations(char *input)
{
	int	i;
	int	single_quote_open;
	int	double_quote_open;

	i = 0;
	single_quote_open = 0;
	double_quote_open = 0;
	while (input[i])
	{
		if (input[i] == '"' && !single_quote_open)
			double_quote_open = !double_quote_open;
		else if (input[i] == '\'' && !double_quote_open)
			single_quote_open = !single_quote_open;
		i++;
	}
	if (single_quote_open || double_quote_open)
		return (0);
	return (1);
}

int	if_error(int flag, t_token *token, char *input, t_env_cpy *env_cpy)
{
	if (flag)
	{
		free_token_list(token);
		env_cpy->last_exit_status = 2;
		return (1);
	}
	if (!check_for_quotations(input))
	{
		error_occurd_status("Syntax error: unmatched quotes\n", 2, env_cpy);
		free_token_list(token);
		return (1);
	}
	if (is_invalid_pipe_syntax(token))
	{
		error_occurd_status("Minishell: syntax error near"
			" unexpected token `|'\n", 2, env_cpy);
		free_token_list(token);
		return (1);
	}
	return (0);
}
