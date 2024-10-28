/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mustafa-machlouch <mustafa-machlouch@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:07:00 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/10/27 10:36:15 by mustafa-mac      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static void	free_dest(char ***dest, int *i)
{
	while ((*i) > 0)
		free((*dest)[--(*i)]);
	free((*dest));
}

static int	fill_combine(char **combine, t_env_cpy *env, char ***dest, int *i)
{
	if (env->equal)
		(*combine) = ft_strjoin(env->type, "=");
	else
		(*combine) = ft_strjoin(env->type, "");
	if (!(*combine))
	{
		free_dest(dest, i);
		return (1);
	}
	return (0);
}

static int	env_loop_to_2d(char **combine, t_env_cpy *env, char ***dest, int *i)
{
	char	*env_value;

	if (fill_combine(combine, env, dest, i))
		return (1);
	if (env->env)
		env_value = ft_strdup(env->env);
	else
		env_value = ft_strdup("");
	if (!env_value)
	{
		free((*combine));
		free_dest(dest, i);
		return (1);
	}
	(*dest)[(*i)] = ft_strjoin((*combine), env_value);
	free((*combine));
	free(env_value);
	if (!(*dest)[(*i)])
	{
		free_dest(dest, i);
		return (1);
	}
	(*i)++;
	return (0);
}

char	**list_to_2d(t_env_cpy *env)
{
	t_env_cpy	*head;
	char		**dest;
	char		*combine;
	int			i;

	i = 0;
	head = env;
	while (env)
	{
		i++;
		env = env->next;
	}
	dest = malloc(sizeof(char *) * (i + 1));
	if (!dest)
		return (NULL);
	i = 0;
	env = head;
	while (env)
	{
		if (env_loop_to_2d(&combine, env, &dest, &i))
			return (NULL);
		env = env->next;
	}
	dest[i] = NULL;
	return (dest);
}
