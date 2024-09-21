/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 15:24:25 by mmachlou          #+#    #+#             */
/*   Updated: 2024/09/21 19:59:45 by mohamibr         ###   ########.fr       */
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
    void    (*prev_sigint_handler)(int);

    env = list_to_2d(env_cpy);
    av[0] = token->tokens;
    av[1] = NULL;

    // Determine the command path
    if (ft_strncmp(token->tokens, "/", 1) == 0
        || ft_strncmp(token->tokens, "./", 2) == 0
        || ft_strncmp(token->tokens, "../", 3) == 0)
        cmd_path = ft_strdup(token->tokens);
    else
        cmd_path = find_in_path(token->tokens, env_cpy); 
    
    if (!cmd_path)
    {
        fprintf(stderr, "%s: command not found\n", token->tokens);
        env_cpy->last_exit_status = 127;  // Command not found
        ft_free_2darray(env);
        return;
    }

    // Fork the process to execute the command
    pid = fork();
    if (pid == 0)
    {
        // In child process

        // Reset signal handlers to default in the child process
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);

        if (execve(cmd_path, av, env) == -1)
        {
            perror("Execution error");
            ft_free_2darray(env);
            free(cmd_path);
            exit(EXIT_FAILURE);
        }
    }
    else if (pid > 0)
    {
        // In parent process

        // Ignore SIGINT in the parent process while the child is running
        prev_sigint_handler = signal(SIGINT, SIG_IGN);

        // Wait for the child process to finish
        waitpid(pid, &status, 0);

        // Restore the original SIGINT handler
        signal(SIGINT, prev_sigint_handler);

        // Handle exit status
        if (WIFEXITED(status))
            env_cpy->last_exit_status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
        {
            int sig = WTERMSIG(status);
            env_cpy->last_exit_status = 128 + sig;

            // Optionally print a message for certain signals
            if (sig == SIGINT)
                write(1, "\n", 1);
            else if (sig == SIGQUIT)
                write(1, "Quit: 3\n", 8);
        }
    }
    else
    {
        perror("fork");
        env_cpy->last_exit_status = 1;
    }

    free(cmd_path);
    ft_free_2darray(env);
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

