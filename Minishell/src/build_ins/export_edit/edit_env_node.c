/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edit_env_node.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 07:20:15 by mohamibr          #+#    #+#             */
/*   Updated: 2024/10/18 07:20:18 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

void	update_nd(t_env_cpy *node, t_env_cpy *head, char *env, bool equal)
{
	char	*new_env;

	if (equal)
	{
		free(node->env);
		if (env)
		{
			if (find_dollar(env))
			{
				new_env = test_dollar(env, head);
				if (!new_env)
					new_env = ft_strdup("");
				env = new_env;
			}
			node->env = ft_strdup(env);
			if (find_dollar(env))
				free(env);
		}
		else
			node->env = ft_strdup("");
		node->equal = true;
	}
}

t_env_cpy	*fill_else_new_node(t_env_cpy *new_node, bool equal)
{
	new_node->equal = equal;
	new_node->next = NULL;
	new_node->previous = NULL;
	new_node->heredoc_file = NULL;
	new_node->last_exit_status = 0;
	new_node->last_output_fd = -1;
	new_node->last_input_fd = -1;
	new_node->internal_pwd = NULL;
	new_node->internal_oldpwd = NULL;
	new_node->flag = 0;
	return (new_node);
}

t_env_cpy	*if_equal_new_node(t_env_cpy *new_node, char *env)
{
	if (env)
		new_node->env = ft_strdup(env);
	else
		new_node->env = ft_strdup("");
	return (new_node);
}

t_env_cpy	*create_env_node(char *type, char *env, bool equal)
{
	t_env_cpy	*new_node;

	new_node = malloc(sizeof(t_env_cpy));
	if (!new_node)
		return (NULL);
	new_node->type = ft_strdup(type);
	if (!new_node->type)
	{
		free(new_node);
		return (NULL);
	}
	if (equal)
	{
		new_node = if_equal_new_node(new_node, env);
		if (!new_node->env)
		{
			free(new_node->type);
			free(new_node);
			return (NULL);
		}
	}
	else
		new_node->env = NULL;
	new_node = fill_else_new_node(new_node, equal);
	return (new_node);
}

void	add_env_node(t_env_cpy **head, t_env_cpy *new_node)
{
	t_env_cpy	*tmp;

	if (!*head)
		*head = new_node;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_node;
		new_node->previous = tmp;
	}
}
//stop!!
