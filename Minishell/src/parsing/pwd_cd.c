/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 10:08:51 by mohamibr          #+#    #+#             */
/*   Updated: 2024/09/17 21:18:01 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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
			printf("cd; OLDPWD not set\n");
			return (NULL);
		}
		printf("%s\n", get_old_path(env_cpy, "OLDPWD"));
	}
	else if (ft_strcmp(token->next->tokens, ".") == 0)
		path = get_old_path(env_cpy, "PWD");
	else
		path = token->next->tokens;
	return (path);
}

void	ft_pwd(t_token *token)
{
	char	*pwd;

	if (token->next != NULL)
		printf("Error: too many arguments\n");
	else
	{
		pwd = getcwd(NULL, 0);
		printf("%s\n", pwd);
		free(pwd);
	}
}



void	ft_cd(t_token *token, t_env_cpy *env_cpy)
{
	char	*path;

	update_env(env_cpy);
	if (token->next && token->next->next)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		return ;
	}
	path = get_cd_path(token, env_cpy);
	if (!path)
		return ;
	if (chdir(path) == -1)
		perror("cd");
}

