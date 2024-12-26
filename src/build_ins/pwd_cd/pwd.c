/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 07:45:37 by mohamibr          #+#    #+#             */
/*   Updated: 2024/12/26 11:11:39 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

char	*get_pwd(t_env_cpy *env_cpy)
{
	char	*pwd;

	pwd = get_env_msg(env_cpy, "PWD");
	if (pwd)
		return (pwd);
	if (env_cpy->internal_pwd)
		return (ft_strdup(env_cpy->internal_pwd));
	return (NULL);
}

char	*get_oldpwd(t_env_cpy *env_cpy)
{
	char	*oldpwd;

	oldpwd = get_env_msg(env_cpy, "OLDPWD");
	if (oldpwd)
		return (oldpwd);
	if (env_cpy->internal_oldpwd)
		return (ft_strdup(env_cpy->internal_oldpwd));
	return (NULL);
}

int	check_after_pwd(t_token *head)
{
	if (ft_strcmp(head->tokens, "pwd") == 0)
	{
		if (head->next)
		{
			if ((ft_strncmp(head->next->tokens, "-", 1) == 0)
				|| (ft_strncmp(head->next->tokens, "--", 2) == 0))
				return (1);
			else
				return (0);
		}
	}
	return (0);
}

// pwd helper
void	if_pwd_avialable(t_env_cpy *env, char *pwd)
{
	char	*hello;

	ft_printf("%s\n", pwd);
	update_env_var(env, "PWD", pwd);
	hello = get_env_msg(env, "PWD");
	if (!hello)
	{
		free(env->internal_pwd);
		env->internal_pwd = ft_strdup(pwd);
	}
	free(hello);
}

void	ft_pwd(t_token *token, t_env_cpy *env)
{
	char	*pwd;
	char	*env_pwd;

	if (check_after_pwd(token))
	{
		error_mes_after_checking(token, env);
		return ;
	}
	pwd = getcwd(NULL, 0);
	if (pwd)
	{
		if_pwd_avialable(env, pwd);
		free(pwd);
	}
	else
	{
		env_pwd = get_pwd(env);
		if (env_pwd)
		{
			ft_printf("%s\n", env_pwd);
			free(env_pwd);
		}
		else
			perror("pwd");
	}
}
//stop!
