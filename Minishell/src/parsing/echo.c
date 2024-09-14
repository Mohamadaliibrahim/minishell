/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmachlou <mmachlou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 10:08:35 by mohamibr          #+#    #+#             */
/*   Updated: 2024/09/14 16:56:55 by mmachlou         ###   ########.fr       */
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

void	check_echo(t_token *token)
{
	bool	n;
	int		first;

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
		printf("%s", token->tokens);
		first = 0;
		token = token->next;
	}
	if (!n)
		printf("\n");
}
