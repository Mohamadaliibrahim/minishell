/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 10:08:51 by mohamibr          #+#    #+#             */
/*   Updated: 2024/09/21 17:23:20 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	*last_directory = NULL;

char	*get_old_path(t_env_cpy *env_cpy, char *msg)
{
	while (env_cpy)
	{
		if (ft_strcmp(env_cpy->type, msg) == 0)
			return (env_cpy->env);
		env_cpy = env_cpy->next;
	}
	return (NULL);
}

static char	*get_cd_path(t_token *token, t_env_cpy *env_cpy)
{
	char	*path;

	if (token->next == NULL || ft_strcmp(token->next->tokens, "~") == 0)
	{
		path = get_old_path(env_cpy, "HOME");
		if (!path)
		{
			ft_putstr_fd("cd: HOME not set\n", 2);
			return (NULL);
		}
	}
	else if (ft_strcmp(token->next->tokens, "-") == 0)
	{
		path = get_old_path(env_cpy, "OLDPWD");
		if (!path)
		{
			if (last_directory)
			{
				path = last_directory;
			}
			else
			{
				printf("cd: OLDPWD not set\n");
				return (NULL);
			}
		}
		printf("%s\n", path);
	}
	else if (ft_strcmp(token->next->tokens, ".") == 0)
		path = get_old_path(env_cpy, "PWD");
	else
		path = token->next->tokens;
	return (path);
}

void	ft_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	printf("%s\n", pwd);
	free(pwd);
}

void	update_env_var(t_env_cpy *env_cpy, char *key, char *value)
{
	while (env_cpy)
	{
		if (ft_strcmp(env_cpy->type, key) == 0)
		{
			free(env_cpy->env);
			env_cpy->env = ft_strdup(value);
			return ;
		}
		env_cpy = env_cpy->next;
	}
}

void	ft_cd(t_token *token, t_env_cpy *env_cpy)
{
	char *path;
	char *old_pwd;
	char *new_pwd;

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
	if (chdir(path) == -1)
	{
		perror("cd");
		env_cpy->last_exit_status = 1;
		free(old_pwd);
		return ;
	}
	new_pwd = getcwd(NULL, 0);
	update_env_var(env_cpy, "OLDPWD", old_pwd);
	update_env_var(env_cpy, "PWD", new_pwd);

	// Update the last_directory variable
	if (last_directory)
		free(last_directory);
	last_directory = old_pwd;

	free(new_pwd);
}
