/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 08:56:42 by mohamibr          #+#    #+#             */
/*   Updated: 2024/10/23 08:56:45 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

void	error_happend(void)
{
	perror("Memory allocation error");
	exit(EXIT_FAILURE);
}

bool	check_for_equal(char *env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (env[i] == '=')
			return (true);
		i++;
	}
	return (false);
}

void	check_x(int	*x)
{
	if ((*x) <= 0)
		(*x) = 1;
	else
		(*x) += 1;
	if ((*x) > INT_MAX)
		(*x) = INT_MAX;
	else
		(*x) = (*x);
}

void	check_head(t_env_cpy **head, t_env_cpy **new_node)
{
	if ((*head))
		(*head)->previous = (*new_node);
}

void	else_in_env_loop(t_env_cpy **current, t_env_cpy **new_node)
{
	(*current)->next = (*new_node);
	(*new_node)->previous = (*current);
	(*current) = (*new_node);
}
//stop!!
