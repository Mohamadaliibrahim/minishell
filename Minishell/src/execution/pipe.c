/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:14:50 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/10/22 15:28:13 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_command **parse_commands(t_token *token_list, int *num_commands, t_env_cpy *env_cpy)
{
    t_command   **commands;
    t_token     *current;
    t_token     *cmd_tokens;
    int         count;
    int         i;

    // Count the number of commands
    count = 1;
    current = token_list;
    while (current)
    {
        if (current->token_type == PIPE)
            count++;
        current = current->next;
    }

    *num_commands = count;
    commands = malloc(sizeof(t_command *) * (count + 1)); // +1 for NULL termination
    if (!commands)
        return (NULL);

    // Split tokens into commands
    current = token_list;
    i = 0;
    while (i < count)
    {
        cmd_tokens = NULL;
        commands[i] = malloc(sizeof(t_command));
        if (!commands[i])
        {
            free_commands(commands); // Free all previously allocated commands
            return (NULL);
        }  
        commands[i]->infile = NULL;
        commands[i]->outfile = NULL;
        commands[i]->append = 0;

        while (current && current->token_type != PIPE)
        {
            if (current->token_type == REDIRECT_IN)  // Input redirection <
            {
                current = current->next;
                if (current && current->tokens)
                    commands[i]->infile = ft_strdup(current->tokens);  // Store input file
            }
            else if (current->token_type == REDIRECT_OUT)  // Output redirection >
            {
                current = current->next;
                if (current && current->tokens)
                    commands[i]->outfile = ft_strdup(current->tokens);  // Store output file
                commands[i]->append = 0;
            }
            else if (current->token_type == APPEND)  // Append redirection >>
            {
                current = current->next;
                if (current && current->tokens)
                    commands[i]->outfile = ft_strdup(current->tokens);  // Store output file
                commands[i]->append = 1;
            }
            else
            {
                // Add other tokens as part of the command
                add_token(&cmd_tokens, current->tokens, env_cpy, current->qoute_type);
            }
            current = current->next;
        }

        commands[i]->token_list = cmd_tokens;
        commands[i]->argv = allocate_arguments(cmd_tokens);

        if (current && current->token_type == PIPE)  // Move past PIPE
            current = current->next;

        i++;
    }
    commands[i] = NULL;  // Null-terminate the array
    return commands;
}


void execute_pipeline(t_token *token_list, t_env_cpy *env_cpy)
{
    int         num_commands;
    t_command   **commands;
    int         num_pipes;
    int         **pipes;
    pid_t       *pids;
    int         i;
    int         infile_fd;
    int         outfile_fd;

    // Parse commands
    commands = parse_commands(token_list, &num_commands, env_cpy);
    if (!commands)
        return; // Handle parsing error

    num_pipes = num_commands - 1;
    pipes = create_pipes(num_pipes);  // Allocate pipes
    if (!pipes)
    {
        free_commands(commands);
        return; // Handle pipe creation error
    }

    pids = malloc(sizeof(pid_t) * num_commands);
    if (!pids)
    {
        free_commands(commands);
        free_pipes(pipes, num_pipes);  // Free pipes in case of failure
        return; // Handle allocation failure
    }

    for (i = 0; i < num_commands; i++)
    {
        pids[i] = fork();
        if (pids[i] == -1)
        {
            perror("fork");
            env_cpy->last_exit_status = 1;
            break;  // On fork failure, ensure pipes and commands are freed
        }

        if (pids[i] == 0)  // In child process
        {
            // Handle input redirection if any, including heredoc
            if (commands[i]->infile || (env_cpy->heredoc_file && i == 0))
            {
                char *input_file = commands[i]->infile ? commands[i]->infile : env_cpy->heredoc_file;
                infile_fd = open(input_file, O_RDONLY);
                if (infile_fd < 0)
                {
                    perror(input_file);
                    free_pipes(pipes, num_pipes);  // Free pipes in child process
                    free_pids_and_commands(pids, commands);  // Free other resources
                    exit(1);
                }
                dup2(infile_fd, STDIN_FILENO);  // Redirect stdin from the input file
                close(infile_fd);
            }
            else if (i > 0)  // If no input redirection, use pipe
            {
                dup2(pipes[i - 1][0], STDIN_FILENO);  // Connect stdin to the previous pipe
            }

            // Handle output redirection if any
            if (commands[i]->outfile)
            {
                if (commands[i]->append)
                    outfile_fd = open(commands[i]->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
                else
                    outfile_fd = open(commands[i]->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);

                if (outfile_fd < 0)
                {
                    perror(commands[i]->outfile);
                    free_pipes(pipes, num_pipes);  // Free pipes in child process
                    free_pids_and_commands(pids, commands);  // Free other resources
                    exit(EXIT_FAILURE);
                }
                dup2(outfile_fd, STDOUT_FILENO);  // Redirect stdout to the file
                close(outfile_fd);
            }
            else if (i < num_commands - 1)  // If no output redirection, use pipe
            {
                dup2(pipes[i][1], STDOUT_FILENO);  // Connect stdout to the next pipe
            }

            // Close all pipe file descriptors in child
            close_pipes(pipes, num_pipes);

            // Execute the command
            ft_cmd(commands[i]->token_list, env_cpy, 0);

            // If exec fails
            free_pipes(pipes, num_pipes);  // Free pipes
            free_pids_and_commands(pids, commands);  // Free other resources
            exit(env_cpy->last_exit_status);
        }
        else
        {
            // Close the pipe ends in the parent process
            if (i > 0)
                close(pipes[i - 1][0]);  // Close the read end of the previous pipe
            if (i < num_commands - 1)
                close(pipes[i][1]);  // Close the write end of the current pipe
        }
    }

    // Parent process closes all pipe file descriptors
    close_pipes(pipes, num_pipes);

    // Wait for all child processes to finish
    for (i = 0; i < num_commands; i++)
    {
        int status;
        waitpid(pids[i], &status, 0);
        if (WIFEXITED(status))
            env_cpy->last_exit_status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
        {
            int sig = WTERMSIG(status);
            env_cpy->last_exit_status = 128 + sig;
            if (sig == SIGINT)
                write(1, "\n", 1);
            else if (sig == SIGQUIT)
                write(1, "Quit: 3\n", 8);
        }
    }

    // Free allocated memory
    free(pids);
    free_commands(commands);
    free_pipes(pipes, num_pipes);  // Ensure pipes are always freed

    // Unlink the heredoc file after all commands are finished
    if (env_cpy->heredoc_file)
    {
        unlink(env_cpy->heredoc_file);
        free(env_cpy->heredoc_file);
        env_cpy->heredoc_file = NULL;
    }
}
