/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mustafa-machlouch <mustafa-machlouch@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:14:50 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/09/28 14:23:12 by mustafa-mac      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char *get_command_path_simple(char *command, t_env_cpy *env_cpy) {
    char *path_env = getenv("PATH");
    (void)env_cpy;  // Unused parameter
    if (!path_env) {
        return NULL;  // No PATH environment variable set
    }

    char *path = ft_strdup(path_env);
    if (!path) {
        return NULL;  // Memory allocation failed
    }

    char *token = strtok(path, ":");
    while (token != NULL) {
        char *cmd_path = ft_strjoin(token, "/");
        char *full_path = ft_strjoin(cmd_path, command);
        free(cmd_path);

        // Check if the command exists and is executable
        if (access(full_path, X_OK) == 0) {
            free(path);  // Free the duplicated PATH string
            return full_path;  // Return the full path to the command
        }

        free(full_path);  // Command not found in this directory, try the next
        token = strtok(NULL, ":");
    }

    free(path);
    return NULL;  // Command not found in any PATH directories
}


void pipe_commands(t_token *token, t_env_cpy *env_cpy) {
    int pipefd[2];
    int fd_in = 0;  // Input file descriptor for the next command
    pid_t pid;

    t_token *current = token;

    while (current) {
        if (pipe(pipefd) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        if ((pid = fork()) == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process
            if (fd_in != 0) {
                dup2(fd_in, STDIN_FILENO);  // Redirect input from previous command
            }
            if (current->next && current->next->token_type == PIPE) {
                dup2(pipefd[1], STDOUT_FILENO);  // Redirect output to next command
            }

            close(pipefd[0]);  // Close unused read end of the pipe
            close(pipefd[1]);  // Close unused write end of the pipe

            // Prepare command and arguments manually
            char *command = current->tokens;
            char *cmd_path = get_command_path_simple(command, env_cpy);  // Assuming you have a simpler function

            // Prepare arguments manually (simplified example)
            char *av[] = {cmd_path, NULL};  // You would need to expand this to handle arguments correctly
            char **env = list_to_2d(env_cpy);

            execve(cmd_path, av, env);  // Execute the command
            perror("execve failed");
            exit(EXIT_FAILURE);
        } else {
            // Parent process
            waitpid(pid, NULL, 0);  // Wait for child process to finish
            close(pipefd[1]);  // Close the write end of the pipe
            fd_in = pipefd[0];  // Keep the read end of the pipe for the next command
            current = current->next;  // Move to the next token

            // Skip over the pipe token
            if (current && current->token_type == PIPE) {
                current = current->next;
            }
        }
    }

    close(fd_in);  // Close the last input file descriptor
}
