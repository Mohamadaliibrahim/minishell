/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 10:49:47 by mohamibr          #+#    #+#             */
/*   Updated: 2024/10/12 14:52:01 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char **allocate_arguments(t_token *token)
{
	t_token	*current;
	char	**av;
	int		token_count;
	int		i;

	// Count the number of tokens
	token_count = 0;
	current = token;
	while (current)
	{
		token_count++;
		current = current->next;
	}

	// Allocate space for arguments array + 1 for the NULL terminator
	av = (char **)malloc((token_count + 1) * sizeof(char *));
	if (!av)
		return (NULL); // Handle memory allocation failure

	// Populate the arguments array
	current = token;
	i = 0;
	while (current)
	{
		av[i] = ft_strdup(current->tokens);
		if (!av[i])
		{
			// Handle memory allocation failure by freeing previously allocated memory
			while (i > 0)
				free(av[--i]);
			free(av);
			return (NULL);
		}
		current = current->next;
		i++;
	}
	av[i] = NULL; // Null-terminate the array
	return (av);
}

char *get_command_path(char **av, t_env_cpy *env_cpy)
{
	char *cmd_path;

	if (ft_strncmp(av[0], "/", 1) == 0 || ft_strncmp(av[0], "./", 2) == 0
		|| ft_strncmp(av[0], "../", 3) == 0)
		cmd_path = ft_strdup(av[0]);
	else
		cmd_path = find_in_path(av[0], env_cpy);

	if (!cmd_path)
	{
		fprintf(stderr, "%s: command not found\n", av[0]);
		env_cpy->last_exit_status = 127; // Command not found
	}
	return (cmd_path);
}

void execute_command(char *cmd_path, char **av, char **env, t_env_cpy *env_cpy)
{
	int infile_fd;

	infile_fd = -1;

	// Handle heredoc redirection
	if (env_cpy->heredoc_file)
	{
		infile_fd = open(env_cpy->heredoc_file, O_RDONLY);
		if (infile_fd < 0)
		{
			perror("minishell: open heredoc file");
			exit(EXIT_FAILURE);
		}
		dup2(infile_fd, STDIN_FILENO);  // Redirect stdin to the heredoc file
		close(infile_fd);
	}

	// Reset signal handlers to default in the child process
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);

	// Execute the command
	if (execve(cmd_path, av, env) == -1)
	{
		perror("Execution error");
		ft_free_2darray(env);
		ft_free_2darray(av);
		free(cmd_path);
		exit(EXIT_FAILURE);
	}
}

static void handle_parent_process(int pid, t_env_cpy *env_cpy)
{
	int status;
	void (*prev_sigint_handler)(int);
	int sig;

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
		sig = WTERMSIG(status);
		env_cpy->last_exit_status = 128 + sig;
		if (sig == SIGINT)
			write(1, "\n", 1);
		else if (sig == SIGQUIT)
			write(1, "Quit: 3\n", 8);
	}
}

void do_comand(t_token *token, t_env_cpy *env_cpy)
{
	int		pid;
	char	*cmd_path;
	char	**env;
	char	**av;

	env = list_to_2d(env_cpy);
	//env_cpy->heredoc_file = 0;
	av = allocate_arguments(token);
	if (!av)
	{
		ft_free_2darray(env);
		return ; // Handle memory allocation failure
	}
	cmd_path = get_command_path(av, env_cpy);
	if (!cmd_path)
	{
		ft_free_2darray(env);
		ft_free_2darray(av);
		return ;
	}

	// Fork the process to execute the command
	pid = fork();
	if (pid == 0)
	{
		// In child process
		
		execute_command(cmd_path, av, env, env_cpy);
	}
	else if (pid > 0)
	{
		// In parent process
		handle_parent_process(pid, env_cpy);
	}
	else
	{
		perror("fork");
		env_cpy->last_exit_status = 1;
	}

	// Clean up
	free(cmd_path);
	ft_free_2darray(env);
	ft_free_2darray(av);

	// Unlink the heredoc file after use
	if (env_cpy->heredoc_file)
	{
		unlink(env_cpy->heredoc_file);  // Remove the temporary heredoc file
		free(env_cpy->heredoc_file);  // Free the heredoc file path memory
		env_cpy->heredoc_file = NULL;  // Reset the pointer in the environment struct
	}
}
