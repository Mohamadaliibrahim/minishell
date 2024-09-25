/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 20:40:37 by mohamibr          #+#    #+#             */
/*   Updated: 2024/09/26 00:52:05 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// char	*get_pwd(t_env_cpy *tmp)
// {
// 	t_env_cpy	*env;

// 	env = tmp;
// 	while (env)
// 	{
// 		if (ft_strcmp(env->type, "PWD") == 0)
// 			return (ft_strdup(env->env));
// 		env = env->next;
// 	}
// 	return (NULL);
// }

// char	*get_old_path(t_env_cpy *env_cpy, char *msg)
// {
// 	while (env_cpy)
// 	{
// 		if (ft_strcmp(env_cpy->type, msg) == 0)
// 			return (env_cpy->env);
// 		env_cpy = env_cpy->next;
// 	}
// 	return (NULL);
// }

t_env_cpy	*find_smallest(t_env_cpy *head, t_env_cpy *exclude)
{
	t_env_cpy	*smallest;

	smallest = NULL;
	while (head)
	{
		if ((!exclude || strcmp(head->type, exclude->type) > 0) &&
			(!smallest || strcmp(head->type, smallest->type) < 0))
			smallest = head;
		head = head->next;
	}
	return (smallest);
}

void	print_sorted(t_env_cpy *head)
{
	t_env_cpy	*smallest;
	t_env_cpy	*current;

	current = NULL;
	while (1)
	{
		smallest = find_smallest(head, current);
		if (!smallest)
			break ;
		print_export(smallest);
		current = smallest;
	}
}
