/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 10:08:51 by mohamibr          #+#    #+#             */
/*   Updated: 2024/09/26 00:30:10 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include <sys/stat.h> // For stat
#include <errno.h>    // For errno

static char	*last_directory = NULL;

// Function to retrieve environment variables based on key
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

// Function to determine the path for the cd command
static char	*get_cd_path(t_token *token, t_env_cpy *env_cpy)
{
	char	*path;
	char	*str;

	str = NULL;
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
	{
		path = get_old_path(env_cpy, "OLDPWD");
		if (!path)
		{
			str = get_old_path(env_cpy, "PWD");
			path = ft_strjoin(str, "/..");
			free(str);
		}
	}
	else
		path = token->next->tokens;
	return (path);
}

// Function to retrieve PWD from environment variables
char	*get_pwd(t_env_cpy *tmp)
{
	t_env_cpy	*env;

	env = tmp;
	while (env)
	{
		if (ft_strcmp(env->type, "PWD") == 0)
			return (ft_strdup(env->env));
		env = env->next;
	}
	return (NULL);
}

// Function to handle the pwd command
void	ft_pwd(t_env_cpy *env)
{
	char	*pwd;
	char	*env_pwd;

	pwd = getcwd(NULL, 0);
	if (pwd)
	{
		printf("%s\n", pwd);
		update_env_var(env, "PWD", pwd);
		free(pwd);
	}
	else
	{
		env_pwd = get_pwd(env);
		if (env_pwd)
			printf("%s\n", env_pwd);
		else
			perror("pwd");
	}
}

// Function to update environment variables
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

// Function to handle the cd command
void	ft_cd(t_token *token, t_env_cpy *env_cpy)
{
	char	*path;
	char	*old_pwd;
	char	*new_pwd;
	// struct stat	sb;

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
	if (!old_pwd)
	{
		// If getcwd fails, retrieve PWD from environment
		old_pwd = get_pwd(env_cpy);
		if (!old_pwd)
		{
			ft_putstr_fd("cd: PWD not set\n", 2);
			env_cpy->last_exit_status = 1;
			return;
		}
		old_pwd = ft_strdup(old_pwd);
	}

	// Attempt to change the directory
	if (chdir(path) == -1)
	{
		perror("cd");
		env_cpy->last_exit_status = 1;

		// Check if the command was 'cd ..'
		if (token->next && ft_strcmp(token->next->tokens, "..") == 0)
		{
			// Append "/.." to PWD every time 'cd ..' fails
			char *temp = ft_strjoin(old_pwd, "/..");
			if (temp)
			{
				update_env_var(env_cpy, "PWD", temp);
				printf("%s\n", temp);
				free(temp);
			}
			else
			{
				// If memory allocation fails, retain old PWD
				update_env_var(env_cpy, "PWD", old_pwd);
				printf("%s\n", old_pwd);
			}
		}
		free(old_pwd);
		return;
	}

	// Get the new working directory
	new_pwd = getcwd(NULL, 0);
	if (new_pwd)
	{
		// Update PWD with the new path
		update_env_var(env_cpy, "PWD", new_pwd);
		free(new_pwd);
	}
	else
	{
		// If getcwd fails after successful chdir, likely due to deleted parent
		// Append "/.." to PWD every time 'cd ..' fails
		char *temp = ft_strjoin(old_pwd, "/..");
		if (temp)
		{
			update_env_var(env_cpy, "PWD", temp);
			free(temp);
		}
		else
		{
			// If memory allocation fails, retain old PWD
			update_env_var(env_cpy, "PWD", old_pwd);
		}
	}

	// Update OLDPWD
	update_env_var(env_cpy, "OLDPWD", old_pwd);
	free(old_pwd);
}
