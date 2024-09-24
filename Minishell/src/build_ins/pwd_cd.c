/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 10:08:51 by mohamibr          #+#    #+#             */
/*   Updated: 2024/09/24 20:59:05 by mohamibr         ###   ########.fr       */
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
char	*get_pwd(t_env_cpy *tmp)
{
	t_env_cpy	*env;

	env = tmp;
	while (env)
	{
		if (ft_strcmp(env->type, "PWD") == 0)
			return (env->env);
		env = env->next;
	}
	return (NULL);
}

void	ft_pwd(t_env_cpy *env)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		pwd = get_pwd(env);
	printf("%s\n", pwd);
	free(pwd);
}

void	update_env_var(t_env_cpy *env_cpy, char *key, char *value)
{
	if (value == NULL)
		return ;
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

char	*remove_last(char *str)
{
	int		i;
	int		j;
	char	*dest;

	i = ft_strlen(str) - 1;
	while (i > 0 && (str[i] != '/'))
		i--;
	if (i < 0)
		return (ft_strdup(".."));
	if (i == 0 && str[0] == '/')
		return (ft_strdup("/.."));
	dest = malloc(sizeof(char) * (i + 4));
	if (!dest)
		return (NULL);
	j = 0;
	while (j < i)
	{
		dest[j] = str[j];
		j++;
	}
	dest[j++] = '/';
	dest[j++] = '.';
	dest[j++] = '.';
	dest[j] = '\0';
	free(str);
	return (dest);
}

void	ft_cd(t_token *token, t_env_cpy *env_cpy)
{
	char	*path;
	char	*old_pwd;
	char	*new_pwd;

	update_env(env_cpy);

	// Check for too many arguments
	if (token->next && token->next->next)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		env_cpy->last_exit_status = 1;
		return;
	}

	path = get_cd_path(token, env_cpy);
	if (!path)
		return;

	// Get the current working directory before changing it
	old_pwd = getcwd(NULL, 0);

	// Attempt to change the directory
	if (chdir(path) == -1)
	{
		perror("cd");
		env_cpy->last_exit_status = 1;
		free(old_pwd);
		return;
	}

	// Get the new working directory
	new_pwd = getcwd(NULL, 0);

	// If getcwd fails, use remove_last to construct a logical path
	if (!new_pwd && old_pwd)
		new_pwd = remove_last(old_pwd);

	// Update environment variables
	if (old_pwd)
		update_env_var(env_cpy, "OLDPWD", old_pwd);
	if (new_pwd)
		update_env_var(env_cpy, "PWD", new_pwd);

	// Update the last_directory variable
	if (last_directory)
		free(last_directory);
	last_directory = old_pwd;

	free(new_pwd);
}
