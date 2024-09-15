/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 15:24:25 by mmachlou          #+#    #+#             */
/*   Updated: 2024/09/15 14:53:55 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*find_in_path(char *cmd)
{
	char	*path_env;
	char	**paths;
	char	*full_path;
	int		i;

	path_env = getenv("PATH");
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

int execute_command(char **args, t_shell *shell)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		// Child process
		execve(args[0], args, shell->env);
		// If execve fails
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
	{
		// Fork failed
		perror("fork");
		shell->last_exit_status = 1;
	}
	else
	{
		// Parent process
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			shell->last_exit_status = WEXITSTATUS(status);
		else
			shell->last_exit_status = 1;
	}
	return (shell->last_exit_status);
}

static void	do_comand(t_token *token, char **env)
{
	int		pid;
	int		status;
	char	*av[2];
	char	*cmd_path;

	(void)cmd_path;
	av[0] = token->tokens;
	av[1] = NULL;
	if (ft_strncmp(token->tokens, "/", 1) == 0
		|| ft_strncmp(token->tokens, "./", 2) == 0
		|| ft_strncmp(token->tokens, "../", 3) == 0)
		cmd_path = ft_strdup(token->tokens);
	else
		cmd_path = find_in_path(token->tokens);
	if (!cmd_path)
	{
		printf("Error");
		return ;
	}
	pid = fork();
	if (pid == 0)
	{
		if (execve(cmd_path, av, env) == -1)
			perror("Error");
		exit(EXIT_FAILURE);
	}
	else if (pid)
		waitpid(pid, &status, 0);
	free(cmd_path);
}

void	check_cmnd(char *input, t_token *token, char **env, t_env_cpy *env_cpy)
{
	t_shell	shell;

	init_shell(&shell, env);
	(void)input;
	if ((ft_strcmp(token->tokens, "ls") == 0)
		|| (ft_strcmp(token->tokens, "clear") == 0))
		do_comand(token, env);
	else if ((ft_strcmp(token->tokens, "echo") == 0))
		check_echo(token, &shell);
	else if ((ft_strcmp(token->tokens, "pwd") == 0))
		ft_pwd(token);
	else if ((ft_strcmp(token->tokens, "env") == 0))
		ft_env(token, env_cpy);
	free_shell(&shell);
}
