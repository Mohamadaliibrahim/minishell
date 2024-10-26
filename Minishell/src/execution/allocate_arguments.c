/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 10:49:47 by mohamibr          #+#    #+#             */
/*   Updated: 2024/10/24 10:41:27 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static	int	count_tokens(t_token *token)
{
	int	count;

	count = 0;
	while (token)
	{
		count++;
		token = token->next;
	}
	return (count);
}

static int	populate_arguments(char **av, t_token *token)
{
	int	i;

	i = 0;
	while (token)
	{
		av[i] = ft_strdup(token->tokens);
		if (!av[i])
		{
			while (i > 0)
				free(av[--i]);
			free(av);
			return (0);
		}
		token = token->next;
		i++;
	}
	av[i] = NULL;
	return (1);
}

char	**allocate_arguments(t_token *token)
{
	int		token_count;
	char	**av;

	token_count = count_tokens(token);
	av = (char **)malloc((token_count + 1) * sizeof(char *));
	if (!av)
		return (NULL);
	if (!populate_arguments(av, token))
		return (NULL);
	return (av);
}
