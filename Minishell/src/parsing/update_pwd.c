/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_pwd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 09:45:50 by mohamibr          #+#    #+#             */
/*   Updated: 2024/09/16 09:46:38 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	check_oldpwd(char *str1, char *str2)
{
	if (ft_strcmp(str1, str2) == 0)
		return (0);
	return (1);
}

char	*find_and_set_oldpwd(t_env_cpy *env, char *pwd)
{
	char	*oldpwd;

	oldpwd = NULL;
	while (env)
	{
		if (ft_strcmp(env->type, "PWD") == 0)
		{
			if (check_oldpwd(env->env, pwd))
				return (ft_strdup(env->env));
			else
			{
				free(pwd);
				return (NULL);
			}
		}
		env = env->next;
	}
	return (oldpwd);
}

void	update_pwd_oldpwd(t_env_cpy *env, char *pwd, char *oldpwd)
{
	while (env)
	{
		if (ft_strcmp(env->type, "PWD") == 0)
		{
			free(env->env);
			env->env = pwd;
		}
		else if (ft_strcmp(env->type, "OLDPWD") == 0)
		{
			free(env->env);
			env->env = oldpwd;
		}
		env = env->next;
	}
}

t_env_cpy	*update_env(t_env_cpy *env)
{
	t_env_cpy	*head;
	char		*pwd;
	char		*oldpwd;

	head = env;
	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (head);
	oldpwd = find_and_set_oldpwd(env, pwd);
	if (!oldpwd)
		return (head);
	env = head;
	update_pwd_oldpwd(env, pwd, oldpwd);
	return (head);
}
