/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mustafa-machlouch <mustafa-machlouch@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:14:50 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/09/26 14:15:44 by mustafa-mac      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void pipe_commands(t_token *token, t_env_cpy *env_cpy) {
    int pipefd[2];
    int pid;
    int fd_in = 0;
    t_token *current = token;

    while (current) {
        pipe(pipefd);

        if ((pid = fork()) == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process
            dup2(fd_in, STDIN_FILENO); // Redirect input from the previous command
            if (current->next) {
                dup2(pipefd[1], STDOUT_FILENO); // Redirect output to the next command
            }
            close(pipefd[0]);
            close(pipefd[1]);

            char **av = allocate_arguments(current);
            char **env = list_to_2d(env_cpy);
            char *cmd_path = get_command_path(av, env_cpy);

            execute_command(cmd_path, av, env, env_cpy); // Execute the command
        } else {
            // Parent process
            waitpid(pid, NULL, 0); // Wait for the child process to finish
            close(pipefd[1]);
            fd_in = pipefd[0]; // Save the input for the next command
            current = current->next; // Move to the next command in the pipeline
        }
    }
}
