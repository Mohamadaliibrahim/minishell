/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 10:08:51 by mohamibr          #+#    #+#             */
/*   Updated: 2024/09/26 13:39:23 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include <sys/stat.h> // For stat
#include <errno.h>    // For errno

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

// Function to retrieve environment variables based on key
char	*get_env_var(t_env_cpy *env_cpy, char *key)
{
	while (env_cpy)
	{
		if (ft_strcmp(env_cpy->type, key) == 0)
			return (env_cpy->env);
		env_cpy = env_cpy->next;
	}
	return (NULL);
}

// Function to determine the path for the cd command
static char	*get_cd_path(t_token *token, t_env_cpy *env_cpy)
{
	char	*path;

	if (token->next == NULL || ft_strcmp(token->next->tokens, "~") == 0)
	{
		path = get_env_var(env_cpy, "HOME");
		if (!path)
		{
			ft_putstr_fd("cd: HOME not set\n", 2);
			return (NULL);
		}
	}
	else if (ft_strcmp(token->next->tokens, "-") == 0)
	{
		path = get_env_var(env_cpy, "OLDPWD");
		if (!path)
		{
			ft_putstr_fd("cd: OLDPWD not set\n", 2);
			return (NULL);
		}
		// Removed the printf statement to prevent printing the path before chdir
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
		// Check for invalid flags (e.g., cd -x)
		if (token->next->tokens[0] == '-' && ft_strlen(token->next->tokens) > 1)
		{
			ft_putstr_fd("cd: invalid option\n", 2);
			return (NULL);
		}
		path = token->next->tokens;
	}
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
		// If getcwd fails, use the stored PWD
		env_pwd = get_pwd(env);
		if (env_pwd)
			printf("%s\n", env_pwd);
		else
			perror("pwd");
		free(env_pwd);
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

		// Check if the command was 'cd -'
		if (token->next && ft_strcmp(token->next->tokens, "-") == 0)
		{
			// Do not modify PWD, simply return after error
			free(old_pwd);
			return;
		}
		// Check if the command was 'cd ..'
		else if (token->next && ft_strcmp(token->next->tokens, "..") == 0)
		{
			// Append "/.." to PWD silently
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
				// Optionally, you can print an error message here
			}
		}
		// Check if the command was 'cd .'
		else if (token->next && ft_strcmp(token->next->tokens, ".") == 0)
		{
			// Do not modify PWD, simply return after error
			// Alternatively, handle any specific behavior for 'cd .'
		}
		else
		{
			// For other failed 'cd' commands, do not modify PWD
			// Alternatively, handle other flags if necessary
		}
		free(old_pwd);
		return;
	}

	// Get the new working directory after successful chdir
	new_pwd = getcwd(NULL, 0);
	if (new_pwd)
	{
		// Update OLDPWD
		update_env_var(env_cpy, "OLDPWD", old_pwd);

		// Update PWD with the new path
		update_env_var(env_cpy, "PWD", new_pwd);
		free(new_pwd);

		// If the command was 'cd -', print the new PWD
		if (token->next && ft_strcmp(token->next->tokens, "-") == 0)
		{
			ft_pwd(env_cpy);
		}
	}
	else
	{
		// If getcwd fails after successful chdir, likely due to deleted parent
		// Handle similar to failed 'cd ..' silently
		if (token->next && ft_strcmp(token->next->tokens, "..") == 0)
		{
			char *temp = ft_strjoin(old_pwd, "/..");
			if (temp)
			{
				update_env_var(env_cpy, "PWD", temp);
				printf("cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n");
				free(temp);
			}
			else
			{
				// If memory allocation fails, retain old PWD
				update_env_var(env_cpy, "PWD", old_pwd);
				// Optionally, you can print an error message here
			}
		}
		else
		{
			// For other successful 'cd' commands where getcwd fails
			update_env_var(env_cpy, "PWD", old_pwd);
		}
	}

	// Update OLDPWD
	update_env_var(env_cpy, "OLDPWD", old_pwd);
	free(old_pwd);
}
