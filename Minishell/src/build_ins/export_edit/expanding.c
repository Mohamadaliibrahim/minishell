/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 07:20:25 by mohamibr          #+#    #+#             */
/*   Updated: 2024/10/18 07:20:28 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

int	find_dollar(char *env)
{
	if (env[0] == '$')
		return (1);
	return (0);
}

char	*remove_dollar(char *env)
{
	char	*dest;
	int		i;
	int		j;

	if (env[0] != '$')
		return (ft_strdup(env));
	dest = malloc(sizeof(char) * (ft_strlen(env)));
	if (!dest)
		return (NULL);
	i = 1;
	j = 0;
	while (env[i])
	{
		dest[j] = env[i];
		i++;
		j++;
	}
	dest[j] = '\0';
	return (dest);
}

char	*test_dollar(char *env, t_env_cpy *head)
{
	t_env_cpy	*env_cpy;
	char		*key;

	if (!env)
		return (ft_strdup(""));
	env_cpy = head;
	key = remove_dollar(env);
	if (!key)
		return (ft_strdup(""));
	while (env_cpy)
	{
		if (ft_strcmp(env_cpy->type, key) == 0)
		{
			free(key);
			if (env_cpy->env)
				return (ft_strdup(env_cpy->env));
			else
				return (ft_strdup(""));
		}
		env_cpy = env_cpy->next;
	}
	free(key);
	return (ft_strdup(""));
}
