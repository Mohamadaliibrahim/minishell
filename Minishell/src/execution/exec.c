/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 10:49:47 by mohamibr          #+#    #+#             */
/*   Updated: 2024/09/26 13:33:58 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	do_comand(t_token *token, t_env_cpy *env_cpy)
{
	int		pid;
	int		status;
	char	*cmd_path;
	char	**env;
	char	**av;
	int		i;
	void	(*prev_sigint_handler)(int);
	t_token	*current;
	int		token_count;
	int		sig;
	int		infile_fd = -1;  // File descriptor for heredoc, if used

	env = list_to_2d(env_cpy);

	env_cpy->heredoc_file = 0;
	// Count the number of tokens to allocate space for the argument array
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
		return ; // Handle memory allocation failure

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
			return ;
		}
		current = current->next;
		i++;
	}
	av[i] = NULL; // Null-terminate the array

	// Determine the command path
	if (ft_strncmp(av[0], "/", 1) == 0 || ft_strncmp(av[0], "./", 2) == 0
		|| ft_strncmp(av[0], "../", 3) == 0)
		cmd_path = ft_strdup(av[0]);
	else
		cmd_path = find_in_path(av[0], env_cpy);

	if (!cmd_path)
	{
		fprintf(stderr, "%s: command not found\n", av[0]);
		env_cpy->last_exit_status = 127; // Command not found
		ft_free_2darray(env);
		ft_free_2darray(av);
		return ;
	}

	// Fork the process to execute the command
	pid = fork();
	if (pid == 0)
	{
		// In child process

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
			sig = WTERMSIG(status);
			env_cpy->last_exit_status = 128 + sig;
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
