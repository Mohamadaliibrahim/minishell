/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 10:08:35 by mohamibr          #+#    #+#             */
/*   Updated: 2024/09/21 12:53:45 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static bool	check_n(t_token **token)
{
	bool	n;
	int		i;

	n = false;
	while (*token)
	{
		if ((*token)->tokens[0] == '-' && (*token)->tokens[1] == 'n')
		{
			i = 1;
			while ((*token)->tokens[++i] == 'n')
				;
			if ((*token)->tokens[i] != '\0')
				break ;
			n = true;
			*token = (*token)->next;
		}
		else
			break ;
	}
	return (n);
}

void	check_echo(t_token *token, t_env_cpy *env_list)
{
	bool	n;
	int		first;
	char	*expanded_token;

	if (!token->next)
	{
		printf("\n");
		return ;
	}
	token = token->next;
	n = check_n(&token);
	first = 1;
	while (token)
	{
		if (!first)
			printf(" ");
		expanded_token = expand_token_if_variable(token->tokens, env_list);
		printf("%s", expanded_token);
		first = 0;
		free(expanded_token);
		token = token->next;
	}
	if (!n)
		printf("\n");
}
