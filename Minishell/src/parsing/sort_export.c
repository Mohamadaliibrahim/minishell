/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 20:40:37 by mohamibr          #+#    #+#             */
/*   Updated: 2024/09/15 20:40:41 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"


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
