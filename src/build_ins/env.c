/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 13:12:58 by mohamibr          #+#    #+#             */
/*   Updated: 2024/09/28 18:45:40 by mohamibr         ###   ########.fr       */
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
	t_env_cpy	*original_env_cpy;

	if (!env_check(token))
	{
		env_cpy->last_exit_status = 127;
		printf("env : No such file or directory");
		return ;
	}
	env_cpy = update_env(env_cpy);
	original_env_cpy = env_cpy;
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
	env_cpy = original_env_cpy;
	if (env_cpy)
		env_cpy->last_exit_status = 0;
}
