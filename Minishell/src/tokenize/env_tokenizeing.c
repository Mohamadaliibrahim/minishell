/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_tokenizeing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 13:03:44 by mohamibr          #+#    #+#             */
/*   Updated: 2024/09/15 19:11:10 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*return_path(char *env)
{
	int		i;
	int		start;
	char	*dest;

	i = 0;
	while (env[i] != '=')
		i++;
	dest = malloc(sizeof(char) * (ft_strlen(env) - i + 1));
	if (!dest)
		return (NULL);
	i++;
	start = i;
	i = 0;
	while (env[start])
	{
		dest[i] = env[start];
		i++;
		start++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*return_type(char *env)
{
	int		i;
	char	*dest;

	i = 0;
	while (env[i] != '=')
		i++;
	dest = malloc(sizeof(char) * (i + 1));
	i = 0;
	while (env[i] != '=')
	{
		dest[i] = env[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
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

t_env_cpy	*cpy_env_helper(char *env)
{
	t_env_cpy	*cpy;

	cpy = malloc(sizeof(t_env_cpy));
	if (!cpy)
		return (NULL);
	cpy->env = return_path(env);
	cpy->type = return_type(env);
	cpy->equal = check_for_equal(env);
	cpy->next = NULL;
	cpy->previous = NULL;
	return (cpy);
}

t_env_cpy	*cpy_env(char **env)
{
	int			i;
	t_env_cpy	*head;
	t_env_cpy	*current;
	t_env_cpy	*new_node;

	i = -1;
	new_node = NULL;
	head = NULL;
	current = NULL;
	while (env[++i])
	{
		new_node = cpy_env_helper(env[i]);
		if (!new_node)
			return (NULL);
		if (!head)
		{
			head = new_node;
			current = head;
		}
		else
		{
			current->next = new_node;
			new_node->previous = current;
			current = new_node;
		}
	}
	return (head);
}
