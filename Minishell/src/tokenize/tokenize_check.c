/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_check.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mustafa-machlouch <mustafa-machlouch@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 15:25:04 by mmachlou          #+#    #+#             */
/*   Updated: 2024/10/07 13:34:01 by mustafa-mac      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int search_for_pipe(t_token *token_list)
{
    t_token *current;

    current = token_list;
    while (current)
    {
        if (current->token_type == PIPE)
            return (1);
        current = current->next;
    }
    return (0);
}

int	check_type(char *token, t_env_cpy *env)
{
	char	*cmd_path;

	if (ft_strncmp(token, "/", 1) == 0 || ft_strncmp(token, "./", 2) == 0
		|| ft_strncmp(token, "../", 3) == 0)
	{
		if (access(token, X_OK) == 0)
			return (CMND);
		else
			return (UNKNOWN);
	}
	if ((ft_strcmp(token, "cd") == 0) || (ft_strcmp(token, "export") == 0)
		|| (ft_strcmp(token, "unset") == 0) || (ft_strcmp(token, "exit") == 0)
		|| (ft_strcmp(token, "env") == 0) || (ft_strcmp(token, "echo") == 0)
		|| (ft_strcmp(token, "pwd") == 0))
		return (CMND);
	cmd_path = find_in_path(token, env);
	if (cmd_path != NULL)
	{
		free(cmd_path);
		return (CMND);
	}
	else if (ft_strcmp(token, "|") == 0)
		return (PIPE);
	else if (ft_strcmp(token, "<<") == 0)
		return (HEREDOC);
	else if (ft_strcmp(token, ">>") == 0)
		return (APPEND);
	else if (ft_strcmp(token, "<") == 0)
		return (REDIRECT_IN);
	else if (ft_strcmp(token, ">") == 0)
		return (REDIRECT_OUT);
	else if (ft_strcmp(token, "'") == 0 || ft_strcmp(token, "\"") == 0)
		return (QUOTE);
	return (UNKNOWN);
}

static int	check_for_quotations(char *input)
{
	int	i;
	int	single_quote_open;
	int	double_quote_open;

	i = 0;
	single_quote_open = 0;
	double_quote_open = 0;
	while (input[i])
	{
		if (input[i] == '"' && !single_quote_open)
			double_quote_open = !double_quote_open;
		else if (input[i] == '\'' && !double_quote_open)
			single_quote_open = !single_quote_open;
		i++;
	}
	if (single_quote_open || double_quote_open)
		return (0);
	return (1);
}

char *preprocess_input(char *input)
{
    char *new_input;
    int i = 0, j = 0;
    int len = ft_strlen(input);
    int new_len = len * 2; // Maximum possible length after adding spaces
    new_input = malloc(new_len + 1); // +1 for null terminator
    if (!new_input)
        return NULL;

    while (input[i])
    {
        // Handle quoted strings
        if (input[i] == '\'' || input[i] == '"')
        {
            char quote = input[i++];
            new_input[j++] = quote; // Copy opening quote
            while (input[i] && input[i] != quote)
                new_input[j++] = input[i++];
            if (input[i] == quote)
                new_input[j++] = input[i++]; // Copy closing quote
        }
        // Check for operators outside quotes
        else if (input[i] == '|' || input[i] == '<' || input[i] == '>')
        {
            // Add space before the operator if necessary
            if (i > 0 && input[i - 1] != ' ')
                new_input[j++] = ' ';
            
            // Add the operator
            new_input[j++] = input[i];

            // Handle double-character operators (<<, >>)
            if ((input[i] == '<' || input[i] == '>') && input[i + 1] == input[i])
            {
                i++;
                new_input[j++] = input[i];
            }

            // Add space after the operator if necessary
            if (input[i + 1] && input[i + 1] != ' ')
                new_input[j++] = ' ';
            i++;
        }
        else
        {
            // Copy other characters
            new_input[j++] = input[i++];
        }
    }
    new_input[j] = '\0';

    return new_input;
}

int is_invalid_pipe_syntax(t_token *token_list)
{
    t_token *current = token_list;

    // Check if the first token is a pipe
    if (current->token_type == PIPE)
        return 1;

    // Traverse the token list and check for consecutive pipes
    while (current->next)
    {
        if (current->token_type == PIPE && current->next->token_type == PIPE)
            return 1;  // Consecutive pipes are invalid
        current = current->next;
    }

    // Check if the last token is a pipe
    if (current->token_type == PIPE)
        return 1;

    return 0;  // No invalid pipe usage found
}


void check(char *input, t_env_cpy *env_cpy)
{
    t_token *token = NULL;
    int error_flag = 0;

    // Preprocess input to add spaces around operators
    char *preprocessed_input = preprocess_input(input);
    if (!preprocessed_input)
    {
        fprintf(stderr, "Error: Memory allocation failed\n");
        env_cpy->last_exit_status = 1;
        return;
    }

    // Tokenize the preprocessed input
    tokenize_input(preprocessed_input, &token, env_cpy, &error_flag);
    free(preprocessed_input); // Free the preprocessed input after tokenization

    if (error_flag)
    {
        if (token)
            free_token_list(token);
        env_cpy->last_exit_status = 2;
        return;
    }
    if (!token)
        return;

    // Check for unmatched quotations (this function may need adjustment)
    if (!check_for_quotations(input))
    {
        fprintf(stderr, "Syntax error: unmatched quotes\n");
        free_token_list(token);
        env_cpy->last_exit_status = 2;
        return;
    }
    if (is_invalid_pipe_syntax(token))
    {
        fprintf(stderr, "Minishell: syntax error near unexpected token `|'\n");
        free_token_list(token);
        env_cpy->last_exit_status = 2;
        return;
    }
    // Optionally, print tokens for debugging
    /*
    t_token *current = token;
    int i = 0;
    while (current)
    {
        printf("token[%d]: %s\n", i, current->tokens);
        current = current->next;
        i++;
    }
    */

    if (token)
    {
        if (search_for_pipe(token))
            execute_pipeline(token, env_cpy);  // Execute piped commands
        else if (search_for_redirection(token))
            check_redirections(token, env_cpy);
        else if (token->token_type == CMND)
            ft_cmd(token, env_cpy);
        else if (token->token_type == UNKNOWN)
        {
            fprintf(stderr, "%s: Command not found\n", token->tokens);
            env_cpy->last_exit_status = 127;
        }
    }
    if (token)
        free_token_list(token);
}
