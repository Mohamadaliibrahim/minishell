/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mustafa-machlouch <mustafa-machlouch@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 13:12:58 by mohamibr          #+#    #+#             */
/*   Updated: 2024/09/17 16:01:18 by mustafa-mac      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	env_check(t_token *token)
{
	while (token)
	{
		if (ft_strcmp(token->tokens, "env") != 0)
			return (0);
		token = token->next;
	}
	return (1);
}

void	ft_env(t_token *token, t_env_cpy *env_cpy)
{
	if (!env_check(token))
		return ;
	env_cpy = update_env(env_cpy);
	while (env_cpy)
	{
		if (env_cpy->equal == true)
		{
			printf("%s", env_cpy->type);
			printf("=");
			printf("%s\n", env_cpy->env);
		}
		env_cpy = env_cpy->next;
	}
}