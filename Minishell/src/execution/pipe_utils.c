/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mustafa-machlouch <mustafa-machlouch@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 09:56:49 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/10/05 13:50:00 by mustafa-mac      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// Function to search fdor input redirection (<) in the token list
int search_for_redirection_input(t_token *token_list)
{
    t_token *current = token_list;

    // Loop through the token list
    while (current)
    {
        if (current->token_type == REDIRECT_IN)  // Assuming REDIRECT_IN corresponds to '<'
            return 1;  // Found input redirection
        current = current->next;
    }
    return 0;  // No input redirection found
}

// Function to search for output redirection (>, >>) in the token list
int search_for_redirection_output(t_token *token_list)
{
    t_token *current = token_list;

    // Loop through the token list
    while (current)
    {
        if (current->token_type == REDIRECT_OUT || current->token_type == APPEND)  // Assuming REDIRECT_OUT is '>' and APPEND is '>>'
            return 1;  // Found output redirection
        current = current->next;
    }
    return 0;  // No output redirection found
}


int handle_input_redirection(t_token *token)
{
    // Locate the file associated with '<'
    t_token *current = token;

    while (current)
    {
        if (current->token_type == REDIRECT_IN)  // Found input redirection
        {
            int infile_fd = open(current->next->tokens, O_RDONLY);
            if (infile_fd < 0)
            {
                perror("minishell: open");
                return -1;
            }
            dup2(infile_fd, STDIN_FILENO); // Redirect stdin to the file
            close(infile_fd);
            return 0;
        }
        current = current->next;
    }
    return 0;
}

int handle_output_redirection(t_token *token, int append)
{
    // Locate the file associated with '>' or '>>'
    t_token *current = token;

    while (current)
    {
        if (current->token_type == REDIRECT_OUT || current->token_type == APPEND)  // Found output redirection
        {
            int outfile_fd;

            if (append)
                outfile_fd = open(current->next->tokens, O_WRONLY | O_CREAT | O_APPEND, 0644);
            else
                outfile_fd = open(current->next->tokens, O_WRONLY | O_CREAT | O_TRUNC, 0644);

            if (outfile_fd < 0)
            {
                perror("minishell: open");
                return -1;
            }
            dup2(outfile_fd, STDOUT_FILENO); // Redirect stdout to the file
            close(outfile_fd);
            return 0;
        }
        current = current->next;
    }
    return 0;
}

int **create_pipes(int num_pipes)
{
    int **pipes;
    int i;

    pipes = malloc(sizeof(int *) * num_pipes);
    if (!pipes)
        return (NULL); // Handle allocation failure
    i = 0;
    while (i < num_pipes)
    {
        pipes[i] = malloc(sizeof(int) * 2);
        if (!pipes[i])
            return (NULL); // Handle allocation failure
        if (pipe(pipes[i]) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        i++;
    }
    return (pipes);
}

void close_pipes(int **pipes, int num_pipes)
{
    int i;

    i = 0;
    while (i < num_pipes)
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
        i++;
    }
}

void free_pipes(int **pipes, int num_pipes)
{
    int i;

    i = 0;
    while (i < num_pipes)
    {
        free(pipes[i]);
        i++;
    }
    free(pipes);
}

void free_commands(t_command **commands)
{
    int i;

    i = 0;
    while (commands[i])
    {
        ft_free_2darray(commands[i]->argv);
        free_token_list(commands[i]->token_list);
        free(commands[i]);
        i++;
    }
    free(commands);
}

