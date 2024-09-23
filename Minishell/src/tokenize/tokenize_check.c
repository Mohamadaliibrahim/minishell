/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_check.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mustafa-machlouch <mustafa-machlouch@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 15:25:04 by mmachlou          #+#    #+#             */
/*   Updated: 2024/09/23 15:23:25 by mustafa-mac      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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

void	check(char *input, t_env_cpy *env_cpy)
{
	t_token	*token;
	t_token	*current;
	int		i;
	int		error_flag;

	token = NULL;
	error_flag = 0;
	tokenize_input(input, &token, env_cpy, &error_flag);
	if (error_flag)
	{
		if (token)
			free_token_list(token);
		env_cpy->last_exit_status = 2;
		return ;
	}
	if (!token)
		return ;
	if (!check_for_quotations(input))
	{
		printf("Syntax error: unmatched quotes\n");
		free_token_list(token);
		return ;
	}
	current = token;
	i = 0;
	while (current)
	{
		current = current->next;
		i++;
	}
	if (token)
	{
		if (search_for_redirection(token))
			check_redirections(token, env_cpy);
		else if (token->token_type == CMND)
			ft_cmd(token, env_cpy);
		else if (token->token_type == UNKNOWN)
		{
			fprintf(stderr, "%s: Command not found\n", token->tokens);
			env_cpy->last_exit_status = 127;
		}
	}
	free_token_list(token);
}
