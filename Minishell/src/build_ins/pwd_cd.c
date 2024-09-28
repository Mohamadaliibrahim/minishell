/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 10:08:51 by mohamibr          #+#    #+#             */
/*   Updated: 2024/09/28 18:47:28 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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
			ptr = ft_strdup(env->env);
			if (ptr)
				return (ptr);
			else
				return (NULL);
		}
		env = env->next;
	}
	return (NULL);
}

// hay el function btrj3lk el path lal token
static char	*get_cd_path(t_token *token, t_env_cpy *env_cpy)
{
	char	*path;

	if (token->next == NULL || ft_strcmp(token->next->tokens, "~") == 0)
	{
		path = get_env_msg(env_cpy, "HOME");
		if (!path)
		{
			ft_putstr_fd("cd: HOME not set\n", 2);
			return (NULL);
		}
	}
	else if (ft_strcmp(token->next->tokens, "-") == 0)
	{
		path = get_env_msg(env_cpy, "OLDPWD");
		if (!path)
		{
			ft_putstr_fd("cd: OLDPWD not set\n", 2);
			return (NULL);
		}
	}
	else if (ft_strcmp(token->next->tokens, ".") == 0)
	{
		path = ".";
	}
	else if (ft_strcmp(token->next->tokens, "..") == 0)
	{
		path = "..";
	}
	else
	{
		if (token->next->tokens[0] == '-' && ft_strlen(token->next->tokens) > 1)
		{
			ft_putstr_fd("cd: invalid option\n", 2);
			return (NULL);
		}
		path = token->next->tokens;
	}
	return (path);
}

// hay el function bt3ml she8el el pwd
void	ft_pwd(t_env_cpy *env)
{
	char	*pwd;
	char	*env_pwd;

	pwd = getcwd(NULL, 0);
	if (pwd)
	{
		printf("%s\n", pwd);
		env = update_env_var(env, "PWD", pwd);
		free(pwd);
	}
	else
	{
		env_pwd = get_env_msg(env, "PWD");
		if (env_pwd)
			printf("%s\n", env_pwd);
		else
			perror("pwd");
		free(env_pwd);
	}
}

// hay el function bt3ml update lal env min 5elel el key wele howe el type
// wel value wele howe el env
t_env_cpy	*update_env_var(t_env_cpy *env_cpy, char *key, char *value)
{
	t_env_cpy	*head;

	head = env_cpy;
	while (env_cpy)
	{
		if (ft_strcmp(env_cpy->type, key) == 0)
		{
			if (env_cpy->env)
			{
				free(env_cpy->env);
				env_cpy->equal = true;
				env_cpy->env = ft_strdup(value);
			}
			else if (!env_cpy->env)
			{
				env_cpy->equal = false;
				env_cpy->env = NULL;
			}
			return (head);
		}
		env_cpy = env_cpy->next;
	}
	return (head);
}

// hyde el function bt3ml she8el el cd
void	ft_cd(t_token *token, t_env_cpy *env_cpy)
{
	char	*path;
	char	*old_pwd;
	char	*new_pwd;
	char	*temp;

	update_env(env_cpy);
	if (token->next && token->next->next)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		env_cpy->last_exit_status = 1;
		return ;
	}
	path = get_cd_path(token, env_cpy);
	if (!path)
		return ;
	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
	{
		old_pwd = get_env_msg(env_cpy, "PWD");
		if (!old_pwd)
		{
			ft_putstr_fd("cd: PWD not set\n", 2);
			env_cpy->last_exit_status = 1;
			return ;
		}
		old_pwd = ft_strdup(old_pwd);
	}
	if (chdir(path) == -1)
	{
		perror("cd");
		env_cpy->last_exit_status = 1;
		if (token->next && ft_strcmp(token->next->tokens, "-") == 0)
		{
			free(old_pwd);
			return ;
		}
		else if (token->next && ft_strcmp(token->next->tokens, "..") == 0)
		{
			temp = ft_strjoin(old_pwd, "/..");
			if (temp)
			{
				update_env_var(env_cpy, "PWD", temp);
				free(temp);
			}
			else
			{
				update_env_var(env_cpy, "PWD", old_pwd);
			}
		}
		free(old_pwd);
		return ;
	}
	new_pwd = getcwd(NULL, 0);
	if (new_pwd)
	{
		update_env_var(env_cpy, "OLDPWD", old_pwd);
		update_env_var(env_cpy, "PWD", new_pwd);
		free(new_pwd);
		if (token->next && ft_strcmp(token->next->tokens, "-") == 0)
		{
			ft_pwd(env_cpy);
		}
	}
	else
	{
		if (token->next && ft_strcmp(token->next->tokens, "..") == 0)
		{
			temp = ft_strjoin(old_pwd, "/..");
			if (temp)
			{
				update_env_var(env_cpy, "PWD", temp);
				printf("cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n");
				free(temp);
			}
			else
			{
				update_env_var(env_cpy, "PWD", old_pwd);
			}
		}
		else
		{
			update_env_var(env_cpy, "PWD", old_pwd);
		}
	}
	update_env_var(env_cpy, "OLDPWD", old_pwd);
	free(old_pwd);
}
