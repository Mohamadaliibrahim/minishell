/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_export_edit.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 07:20:48 by mohamibr          #+#    #+#             */
/*   Updated: 2024/10/18 07:20:49 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

t_env_cpy	*find_env_node(t_env_cpy *head, char *type)
{
	t_env_cpy	*tmp;

	tmp = head;
	while (tmp)
	{
		if (ft_strcmp(tmp->type, type) == 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

t_export	init_export(char *env, char *type, bool equal)
{
	t_export	export;

	if (env)
		export.env = ft_strdup(env);
	else
		export.env = NULL;
	export.expanded_env = NULL;
	if (type)
		export.type = ft_strdup(type);
	else
		export.type = NULL;
	export.equal = equal;
	return (export);
}

void	free_export(t_export export, int flag)
{
	if (export.env && flag)
		free(export.env);
	if (export.expanded_env)
		free(export.expanded_env);
	if (export.type)
		free(export.type);
}
