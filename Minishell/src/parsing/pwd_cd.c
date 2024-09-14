/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmachlou <mmachlou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 10:08:51 by mohamibr          #+#    #+#             */
/*   Updated: 2024/09/14 16:56:59 by mmachlou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	*get_cd_path(t_token *token)
{
	char	*path;

	if (token->next == NULL || ft_strcmp(token->next->tokens, "~") == 0)
	{
		path = getenv("HOME");
		if (!path)
		{
			ft_putstr_fd("cd: HOME not set\n", 2);
			return (NULL);
		}
	}
	else if (ft_strcmp(token->next->tokens, ".") == 0)
	{
		path = getenv("PWD");
	}
	else
	{
		path = token->next->tokens;
	}
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

void	ft_cd(t_token *token)
{
	char	*path;
	char	*old_pwd;
	char	*new_pwd;

	if (token->next && token->next->next)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		return ;
	}
	path = get_cd_path(token);
	if (!path)
		return ;
	old_pwd = getenv("PWD");
	if (chdir(path) == -1)
		perror("cd");
	else
	{
		setenv("OLDPWD", old_pwd, 1);
		new_pwd = getcwd(NULL, 0);
		setenv("PWD", new_pwd, 1);
		free(new_pwd);
	}
}

