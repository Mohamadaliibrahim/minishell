/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 15:24:25 by mmachlou          #+#    #+#             */
/*   Updated: 2024/09/19 21:46:32 by mohamibr         ###   ########.fr       */
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

static void	do_comand(t_token *token, t_env_cpy *env_cpy)
{
	int		pid;
	int		status;
	char	*av[2];
	char	*cmd_path;
	char	**env;
	int		i;

	env = list_to_2d(env_cpy);
	(void)cmd_path;
	av[0] = token->tokens;
	av[1] = NULL;
	if (ft_strncmp(token->tokens, "/", 1) == 0
		|| ft_strncmp(token->tokens, "./", 2) == 0
		|| ft_strncmp(token->tokens, "../", 3) == 0)
		cmd_path = ft_strdup(token->tokens);
	else
		cmd_path = find_in_path(token->tokens, env_cpy); 
	if (!cmd_path)
	{
		perror("Command not found");
		// Free the env array and its contents
		for (i = 0; env[i]; i++)
			free(env[i]);
		free(env);
		return ;
	}
	pid = fork();
	if (pid == 0)
	{
		if (execve(cmd_path, av, env) == -1)
			perror("Execution error");
		// Free the env array and its contents before exiting the child process
		for (i = 0; env[i]; i++)
			free(env[i]);
		free(env);
		free(cmd_path);
		exit(EXIT_FAILURE);
	}
	else if (pid)
		waitpid(pid, &status, 0);
	free(cmd_path);
	// Free the env array and its contents in the parent process
	for (i = 0; env[i]; i++)
		free(env[i]);
	free(env);
}


void	ft_cmd(t_token *token, t_env_cpy *env_cpy)
{
	if ((ft_strcmp(token->tokens, "echo") == 0))
		check_echo(token, env_cpy);
	else if ((ft_strcmp(token->tokens, "pwd") == 0))
		ft_pwd();
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
		exit(0);
	}
	else
		do_comand(token, env_cpy);
}
