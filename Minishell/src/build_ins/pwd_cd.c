/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 10:08:51 by mohamibr          #+#    #+#             */
/*   Updated: 2024/10/03 12:15:33 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char *internal_pwd = NULL;
static char *internal_oldpwd = NULL;
static int	flag = 0;

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




char	*get_pwd(t_env_cpy *env_cpy)
{
	char *pwd = get_env_msg(env_cpy, "PWD");
	if (pwd)
		return pwd;
	if (internal_pwd)
		return ft_strdup(internal_pwd);
	return NULL;
}

char	*get_oldpwd(t_env_cpy *env_cpy)
{
	char	*oldpwd;

	oldpwd = get_env_msg(env_cpy, "OLDPWD");
	if (oldpwd)
		return (oldpwd);
	if (internal_oldpwd)
		return (ft_strdup(internal_oldpwd));
	return (NULL);
}

// hay el function btrj3lk el path lal token
static char	*get_cd_path(t_token *token, t_env_cpy *env_cpy, int *should_free)
{
	char	*path;

	*should_free = 0;
	if (token->next == NULL || ft_strcmp(token->next->tokens, "~") == 0)
	{
		path = get_env_msg(env_cpy, "HOME");
		if (!path)
		{
			ft_putstr_fd("cd: HOME not set\n", 2);
			env_cpy->last_exit_status = 1;
			return (NULL);
		}
		*should_free = 1;
	}
	else if (ft_strcmp(token->next->tokens, "-") == 0)
	{
		path = get_oldpwd(env_cpy);
		if (!path)
		{
			ft_putstr_fd("cd: OLDPWD not set\n", 2);
			env_cpy->last_exit_status = 1;
			return (NULL);
		}
		*should_free = 1;
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
			env_cpy->last_exit_status = 1;
			return (NULL);
		}
		path = token->next->tokens;
	}
	return (path);
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

// hay el function bt3ml she8el el pwd
void	ft_pwd(t_env_cpy *env)
{
	char	*pwd;
	char	*env_pwd;
	char	*hello;

	pwd = getcwd(NULL, 0);
	if (pwd)
	{
		printf("%s\n", pwd);
		update_env_var(env, "PWD", pwd);
		hello = get_env_msg(env, "PWD");
		if (!hello)
		{
			free(internal_pwd);
			internal_pwd = ft_strdup(pwd);
		}
		free(hello);
		free(pwd);
	}
	else
	{
		env_pwd = get_pwd(env);
		if (env_pwd)
		{
			printf("%s\n", env_pwd);
			free(env_pwd);
		}
		else
			perror("pwd");
	}
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


// hyde el function bt3ml she8el el cd
void	ft_cd(t_token *token, t_env_cpy *env_cpy)
{
	char	*path;
	char	*old_pwd;
	char	*new_pwd;
	char	*pwd_env;
	char	*oldpwd_env;
	char	*hello;
	int		should_free;

	update_env(env_cpy);
	// Get PWD and OLDPWD from env_cpy
	pwd_env = get_env_msg(env_cpy, "PWD");
	oldpwd_env = get_env_msg(env_cpy, "OLDPWD");
	hello = NULL;
	if (token->next && token->next->next)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		env_cpy->last_exit_status = 1;
		free(pwd_env);
		free(oldpwd_env);
		return ;
	}
	path = get_cd_path(token, env_cpy, &should_free);
	if (!path)
	{
		free(pwd_env);
		free(oldpwd_env);
		return ;
	}
	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
	{
		if (pwd_env)
			old_pwd = ft_strdup(pwd_env);
		else
		{
			ft_putstr_fd("cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n", 2);
			env_cpy->last_exit_status = 1;
			free(pwd_env);
			free(oldpwd_env);
			free(path);
			return ;
		}
	}
	if (chdir(path) == -1)
	{
		perror("cd");
		env_cpy->last_exit_status = 1;
		free(old_pwd);
		free(pwd_env);
		free(oldpwd_env);
		if (should_free)
			free(path);
		return ;
	}
	new_pwd = getcwd(NULL, 0);
	if (new_pwd)
	{
		// Update PWD

		if (pwd_env)
			update_env_var(env_cpy, "PWD", new_pwd);
		else
		{
			if (internal_pwd)
				free(internal_pwd);
			internal_pwd = ft_strdup(new_pwd);
		}
		// Update OLDPWD only if both PWD and OLDPWD exist in env_cpy
		if (pwd_env && oldpwd_env)
		{
			update_env_var(env_cpy, "OLDPWD", pwd_env);
		}
		else
		{
			// Update internal OLDPWD
			if (internal_oldpwd)
				free(internal_oldpwd);
			if (pwd_env)
				internal_oldpwd =ft_strdup(pwd_env);
			else
			{
				internal_oldpwd = ft_strdup(old_pwd);
				remove_env("OLDPWD", &env_cpy);
				if (flag == 0)
				{
					fill_token(env_cpy, "OLDPWD");
					flag++;
				}
				else if (flag > 0)
				{
					hello = ft_strjoin("OLDPWD=", internal_oldpwd);
					fill_token(env_cpy, hello);
					free(hello);
				}
			}
		}
		free(new_pwd);
		if (token->next && ft_strcmp(token->next->tokens, "-") == 0)
			ft_pwd(env_cpy);
	}
	else
	{
		ft_putstr_fd("cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n", 2);
		env_cpy = add_env_pwd(env_cpy);
		if (pwd_env && oldpwd_env)
		{
			update_env_var(env_cpy, "OLDPWD", pwd_env);
		}
		else
		{
			if (internal_oldpwd)
				free(internal_oldpwd);
			internal_oldpwd = pwd_env;
			if (internal_oldpwd)
				ft_strdup(pwd_env);
			else
				ft_strdup(old_pwd);
		}
	}
	// printf("\n%s\n", old_pwd);
	// printf("\n%s\n", pwd_env);
	// printf("\n%s\n", oldpwd_env);
	free(old_pwd);
	if (should_free)
		free(path);
	free(pwd_env);
	free(oldpwd_env);
}
