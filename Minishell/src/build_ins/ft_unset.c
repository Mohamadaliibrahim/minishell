/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 15:13:03 by mohamibr          #+#    #+#             */
/*   Updated: 2024/10/13 07:38:49 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	remove_env(char *type, t_env_cpy **env_cpy)
{
	t_env_cpy	*prev;
	t_env_cpy	*current;

	current = *env_cpy;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->type, type) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env_cpy = current->next;
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

// int	check_for_not_used(char *str)
// {
// 	int	i;

// 	i = 0;
// 	if (!((str[0] >= 'A' && str[0] <= 'Z') || (str[0] >= 'a' && str[0] <= 'z')))
// 		return (1);
// 	while (str[i])
// 	{
// 		if (!((str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= 'a'
// 					&& str[i] <= 'z') || (str[i] >= '0' && str[i] <= '9')
// 				|| str[i] == '_'))
// 			return (1);
// 		i++;
// 	}
// 	return (0);
// }

// int	check_unset(t_token *head, t_env_cpy **env)
// {
// 	t_token	*token;

// 	token = head;
// 	token = token->next;
// 	while (token)
// 	{
// 		if (check_for_not_used(token->tokens))
// 		{
// 			(*env)->last_exit_status = 127;
// 			perror(token->tokens);
// 			return (1);
// 		}
// 		token = token->next;
// 	}
// 	return (0);
// }

void	ft_unset(t_token *token, t_env_cpy **env_cpy)
{
	char	*type;

	if (!token->next)
	{
		if (*env_cpy)
			(*env_cpy)->last_exit_status = 0;
		return ;
	}
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
	if (*env_cpy)
		(*env_cpy)->last_exit_status = 0;
}
