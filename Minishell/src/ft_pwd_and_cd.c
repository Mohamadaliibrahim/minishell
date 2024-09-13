/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd_and_cd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 10:08:51 by mohamibr          #+#    #+#             */
/*   Updated: 2024/09/13 10:57:20 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"


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
	char	*home_dir;

	if (token->next == NULL)
	{
		home_dir = getenv("HOME");
		if (!home_dir)
		{
			perror("cd: HOME not set");
			return ;
		}
		path = home_dir;
	}
	else
		path = token->next->tokens;
	if (chdir(path) == -1)
		perror("cd");
	else
	{
		setenv("OLDPWD", getenv("PWD"), 1);
		setenv("PWD", path, 1);
	}
}
