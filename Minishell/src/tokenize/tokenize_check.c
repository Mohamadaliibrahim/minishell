/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_check.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 15:25:04 by mmachlou          #+#    #+#             */
/*   Updated: 2024/10/20 12:38:48 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	search_for_pipe(t_token *token_list)
{
	t_token	*current;

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
	char	*expanded_token;

	if (ft_strncmp(token, "/", 1) == 0 || ft_strncmp(token, "./", 2) == 0
		|| ft_strncmp(token, "../", 3) == 0)
	{
		if (access(token, X_OK) == 0)
			return (CMND);
		else
			return (UNKNOWN);
	}
	if (token[0] == '$' && ft_strlen(token) > 1)
	{
		expanded_token = get_env_value(token + 1, env);
		if (expanded_token)
		{
			if ((ft_strcmp(expanded_token, "cd") == 0)
				|| (ft_strcmp(expanded_token, "export") == 0)
				|| (ft_strcmp(expanded_token, "unset") == 0)
				|| (ft_strcmp(expanded_token, "exit") == 0)
				|| (ft_strcmp(expanded_token, "env") == 0)
				|| (ft_strcmp(expanded_token, "echo") == 0)
				|| (ft_strcmp(expanded_token, "pwd") == 0))
				return (CMND);
			cmd_path = find_in_path(expanded_token, env);
			if (cmd_path != NULL)
			{
				free(cmd_path);
				return (CMND);
			}
		}
		return (VARIABLE);
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
	if (ft_strcmp(token, "|") == 0)
		return (PIPE);
	if (ft_strcmp(token, "<<") == 0)
		return (HEREDOC);
	if (ft_strcmp(token, ">>") == 0)
		return (APPEND);
	if (ft_strcmp(token, "<") == 0)
		return (REDIRECT_IN);
	if (ft_strcmp(token, ">") == 0)
		return (REDIRECT_OUT);
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

char	*preprocess_input(char *input)
{
	char	*new_input;
	int		i;
	int		j;
	int		len;
	char	quote;
	int		new_len;

	i = 0;
	j = 0;
	len = ft_strlen(input);
	new_len = len * 2;
	new_input = malloc(new_len + 1);
	if (!new_input)
		return (NULL);
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '"')
		{
			quote = input[i++];
			new_input[j++] = quote;
			while (input[i] && input[i] != quote)
				new_input[j++] = input[i++];
			if (input[i] == quote)
				new_input[j++] = input[i++];
		}
		else if (input[i] == '|' || input[i] == '<' || input[i] == '>')
		{
			if (i > 0 && input[i - 1] != ' ')
				new_input[j++] = ' ';
			new_input[j++] = input[i];
			if ((input[i] == '<' || input[i] == '>')
				&& input[i + 1] == input[i])
			{
				i++;
				new_input[j++] = input[i];
			}
			if (input[i + 1] && input[i + 1] != ' ')
				new_input[j++] = ' ';
			i++;
		}
		else
			new_input[j++] = input[i++];
	}
	new_input[j] = '\0';
	return (new_input);
}

int	is_invalid_pipe_syntax(t_token *token_list)
{
	t_token	*current;

	current = token_list;
	if (current->token_type == PIPE)
		return (1);
	while (current->next)
	{
		if (current->token_type == PIPE && current->next->token_type == PIPE)
			return (1);
		current = current->next;
	}
	if (current->token_type == PIPE)
		return (1);
	return (0);
}

int	fix_pipe(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '|')
		{
			if (str[i + 1] == '|' || str[i + 1] == '\0')
			{
				fprintf(stderr,
					"Minishell: syntax error near unexpected token `||'\n");
				return (1);
			}
		}
		i++;
	}
	return (0);
}

void	check(char *input, t_env_cpy *env_cpy)
{
	t_token	*token;
	int		error_flag;
	char	*preprocessed_input;

	token = NULL;
	error_flag = 0;
	if (fix_pipe(input))
		return ;
	preprocessed_input = preprocess_input(input);
	if (!preprocessed_input)
	{
		fprintf(stderr, "Error: Memory allocation failed\n");
		env_cpy->last_exit_status = 1;
		return ;
	}
	tokenize_input(preprocessed_input, &token, env_cpy, &error_flag);
	free(preprocessed_input);
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
		fprintf(stderr, "Syntax error: unmatched quotes\n");
		free_token_list(token);
		env_cpy->last_exit_status = 2;
		return ;
	}
	if (is_invalid_pipe_syntax(token))
	{
		fprintf(stderr, "Minishell: syntax error near unexpected token `|'\n");
		free_token_list(token);
		env_cpy->last_exit_status = 2;
		return ;
	}
	if (token)
	{
		if (check_token(token))
		{
			fprintf(stderr, "zsh: parse error near `\\n'\n");
			env_cpy->last_exit_status = 2;
		}
		else if (search_for_pipe(token))
			execute_pipeline(token, env_cpy);
		else if (search_for_redirection(token))
			check_redirections(&token, env_cpy);
		else if (token->token_type == CMND)
			ft_cmd(token, env_cpy, 1);
		else if (token->token_type == UNKNOWN)
		{
			fprintf(stderr, "%s: Command not found\n", token->tokens);
			env_cpy->last_exit_status = 127;
		}
	}
	if (token)
		free_token_list(token);
}
