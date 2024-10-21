/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_update.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 07:20:37 by mohamibr          #+#    #+#             */
/*   Updated: 2024/10/18 19:28:18 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

t_env_cpy	*if_node(t_export export, t_env_cpy *node, t_env_cpy **head)
{
	if (export.equal && export.expanded_env)
		update_nd(node, *head, export.expanded_env, export.equal);
	else
		update_nd(node, *head, export.env, export.equal);
	free_export(export, 1);
	return (node);
}

void	if_true_export_edit(t_env_cpy **head, t_export *export)
{
	(*export).expanded_env = test_dollar((*export).env, *head);
	if (!(*export).expanded_env)
		(*export).expanded_env = ft_strdup("");
}

void	else_if_in_a_env(t_export *export)
{
	free((*export).env);
	(*export).env = (*export).expanded_env;
	(*export).expanded_env = NULL;
}

t_env_cpy	*a_env(t_env_cpy **head, char *type, char *env, bool equal)
{
	t_env_cpy	*node;
	t_export	export;

	export = init_export(env, type, equal);
	if (export.equal && export.env && find_dollar(export.env))
		if_true_export_edit(head, &export);
	node = find_env_node(*head, export.type);
	if (node)
		return (if_node(export, node, head));
	else
	{
		if (export.equal && export.expanded_env)
			else_if_in_a_env(&export);
		node = create_env_node(export.type, export.env, export.equal);
		if (!node)
		{
			free_export(export, 0);
			return (NULL);
		}
		add_env_node(head, node);
		free_export(export, 1);
		return (node);
	}
}
