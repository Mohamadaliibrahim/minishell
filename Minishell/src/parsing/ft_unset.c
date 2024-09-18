/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 15:13:03 by mohamibr          #+#    #+#             */
/*   Updated: 2024/09/17 20:10:29 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	remove_env(char *type, t_env_cpy *env_cpy)
{
	t_env_cpy	*prev;
	t_env_cpy	*current;

	current = env_cpy;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->type, type) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				env_cpy = current->next;
			if (current->next)
				current->next->previous = prev;
			free(current->env);
			free(current->type);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

void	ft_unset(t_token *token, t_env_cpy *env_cpy)
{
	char	*type;

	if (!token->next)
		return ;
	token = token->next;
	while (token)
	{
		type = return_type(token->tokens);
		if (type)
		{
			remove_env(type, env_cpy);
			free(type);
		}
		token = token->next;
	}
}
