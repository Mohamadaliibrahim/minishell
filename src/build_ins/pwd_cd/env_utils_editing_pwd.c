/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_editing_pwd.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 07:45:12 by mohamibr          #+#    #+#             */
/*   Updated: 2024/10/15 07:45:16 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

// hay el function btraje3lak el path lal msg le bdk tfwto fiha

char	*get_env_msg(t_env_cpy *tmp, char *msg)
{
	t_env_cpy	*env;
	char		*ptr;

	env = tmp;
	ptr = NULL;
	while (env)
	{
		if (ft_strcmp(env->type, msg) == 0)
		{
			if (env->env)
				ptr = ft_strdup(env->env);
			else
				ptr = ft_strdup("");
			return (ptr);
		}
		env = env->next;
	}
	return (NULL);
}

t_env_cpy	*update_env_var(t_env_cpy *env_cpy, char *key, char *value)
{
	t_env_cpy	*head;

	head = env_cpy;
	while (env_cpy)
	{
		if (ft_strcmp(env_cpy->type, key) == 0)
		{
			if (value)
			{
				free(env_cpy->env);
				env_cpy->env = ft_strdup(value);
				env_cpy->equal = true;
			}
			return (head);
		}
		env_cpy = env_cpy->next;
	}
	return (head);
}

t_env_cpy	*add_env_pwd(t_env_cpy *env)
{
	t_env_cpy	*head;
	char		*str;

	head = env;
	str = NULL;
	while (env)
	{
		if (strcmp(env->type, "PWD") == 0)
		{
			if (env->env)
				str = ft_strdup(env->env);
			else
				str = ft_strdup("");
			free(env->env);
			env->env = ft_strjoin(str, "/..");
			free(str);
			return (head);
		}
		env = env->next;
	}
	return (head);
}

t_cd	init_cd(t_token *token, t_env_cpy *env_cpy)
{
	t_cd	cd;

	cd.pwd_env = get_env_msg(env_cpy, "PWD");
	cd.oldpwd_env = get_env_msg(env_cpy, "OLDPWD");
	cd.hello = NULL;
	cd.path = get_cd_path(token, env_cpy, &(cd.should_free));
	cd.old_pwd = getcwd(NULL, 0);
	cd.new_pwd = NULL;
	return (cd);
}

int	old_pwd_is_null(t_cd cd, t_env_cpy *env_cpy)
{
	char	*str;

	str = NULL;
	if (cd.pwd_env)
	{
		str = ft_strdup(cd.pwd_env);
		if (!str)
		{
			perror("Memory allocation error");
			freeing_cd(cd, env_cpy, 1);
			return (1);
		}
		cd.old_pwd = str;
		if (str)
			free(str);
	}
	else
	{
		too_long_error_cd();
		freeing_cd(cd, env_cpy, 1);
		return (1);
	}
	return (0);
}
//stop!
