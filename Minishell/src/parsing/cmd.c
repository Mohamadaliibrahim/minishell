/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 15:24:25 by mmachlou          #+#    #+#             */
/*   Updated: 2024/09/22 10:53:07 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*find_in_path(char *cmd, t_env_cpy *env)
{
	char	*path_env;
	char	**paths;
	char	*full_path;
	int		i;

	path_env = get_old_path(env, "PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	i = -1;
	while (paths[++i])
	{
		full_path = malloc(strlen(paths[i]) + strlen(cmd) + 2);
		if (!full_path)
			return (NULL);
		strcpy(full_path, paths[i]);
		strcat(full_path, "/");
		strcat(full_path, cmd);
		if (access(full_path, X_OK) == 0)
		{
			ft_free_2darray(paths);
			return (full_path);
		}
		free(full_path);
	}
	ft_free_2darray(paths);
	return (NULL);
}





t_env_cpy	*ft_exit(t_token *token, t_env_cpy *env)
{
	int	a;

	a = ft_atoi(token->next->tokens);
	if (token->next && (a) != 0)
		env->last_exit_status = a;
	else if (token->next)
		env->last_exit_status = 2;
	else
		env->last_exit_status = 0;
	exit(env->last_exit_status);
	return (env);
}

void	ft_cmd(t_token *token, t_env_cpy *env_cpy)
{
	if ((ft_strcmp(token->tokens, "echo") == 0))
		check_echo(token, env_cpy);
	else if ((ft_strcmp(token->tokens, "pwd") == 0))
	{
		env_cpy->last_exit_status = 0;
		ft_pwd();
	}
	else if ((ft_strcmp(token->tokens, "env") == 0))
		ft_env(token, env_cpy);
	else if ((ft_strcmp(token->tokens, "export") == 0))
		ft_export(token, env_cpy);
	else if ((ft_strcmp(token->tokens, "unset") == 0))
		ft_unset(token, env_cpy);
	else if ((ft_strcmp(token->tokens, "cd") == 0))
		ft_cd(token, env_cpy);
	else if ((ft_strcmp(token->tokens, "exit") == 0))
	{
		printf("exit\n");
		ft_exit(token, env_cpy);
	}
	else
		do_comand(token, env_cpy);
}

