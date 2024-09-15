/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mustafa-machlouch <mustafa-machlouch@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 10:08:35 by mohamibr          #+#    #+#             */
/*   Updated: 2024/09/15 10:36:04 by mustafa-mac      ###   ########.fr       */
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

void	check_echo(t_token *token, t_shell *shell)
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
		expanded_token = expand_env_var(token->tokens, shell);
		if (!first)
			printf(" ");
		printf("%s", expanded_token);
		first = 0;
		free(expanded_token);
		token = token->next;
	}
	if (!n)
		printf("\n");
}
