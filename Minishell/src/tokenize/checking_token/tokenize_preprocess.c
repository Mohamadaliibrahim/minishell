/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_preprocess.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 15:25:04 by mmachlou          #+#    #+#             */
/*   Updated: 2024/10/23 08:45:58 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

// int	search_for_pipe(t_token *token_list)
// {
// 	t_token	*current;

// 	current = token_list;
// 	while (current)
// 	{
// 		if (current->token_type == PIPE)
// 			return (1);
// 		current = current->next;
// 	}
// 	return (0);
// }

// int	pipe_or_redirection(char *token)
// {
// 	if (ft_strcmp(token, "|") == 0)
// 		return (PIPE);
// 	if (ft_strcmp(token, "<<") == 0)
// 		return (HEREDOC);
// 	if (ft_strcmp(token, ">>") == 0)
// 		return (APPEND);
// 	if (ft_strcmp(token, "<") == 0)
// 		return (REDIRECT_IN);
// 	if (ft_strcmp(token, ">") == 0)
// 		return (REDIRECT_OUT);
// 	return (UNKNOWN);
// }

// int	is_build_ins(char *token)
// {
// 	if ((ft_strcmp(token, "cd") == 0) || (ft_strcmp(token, "export") == 0)
// 		|| (ft_strcmp(token, "unset") == 0) || (ft_strcmp(token, "exit") == 0)
// 		|| (ft_strcmp(token, "env") == 0) || (ft_strcmp(token, "echo") == 0)
// 		|| (ft_strcmp(token, "pwd") == 0))
// 		return (1);
// 	return (0);
// }

// int	check_for_dollar(char *token, t_env_cpy *env, char **cmd_path,
// 		char **expanded_token)
// {
// 	if (token[0] == '$' && ft_strlen(token) > 1)
// 	{
// 		(*expanded_token) = get_env_value(token + 1, env);
// 		if (*expanded_token)
// 		{
// 			(*cmd_path) = find_in_path(*expanded_token, env);
// 			if (*cmd_path != NULL)
// 			{
// 				free(*cmd_path);
// 				return (CMND);
// 			}
// 		}
// 		return (UNKNOWN);
// 	}
// 	return (-1);
// }

// int	check_type(char *token, t_env_cpy *env)
// {
// 	char	*cmd_path;
// 	char	*expanded_token;

// 	if (ft_strncmp(token, "/", 1) == 0 || ft_strncmp(token, "./", 2) == 0
// 		|| ft_strncmp(token, "../", 3) == 0)
// 	{
// 		if (access(token, X_OK) == 0)
// 			return (CMND);
// 		else
// 			return (UNKNOWN);
// 	}
// 	if (check_for_dollar(token, env, &cmd_path, &expanded_token) != -1)
// 		return (check_for_dollar(token, env, &cmd_path, &expanded_token));
// 	if (is_build_ins(token))
// 		return (CMND);
// 	cmd_path = find_in_path(token, env);
// 	if (cmd_path != NULL)
// 	{
// 		free(cmd_path);
// 		return (CMND);
// 	}
// 	return (pipe_or_redirection(token));
// }

// static int	check_for_quotations(char *input)
// {
// 	int	i;
// 	int	single_quote_open;
// 	int	double_quote_open;

// 	i = 0;
// 	single_quote_open = 0;
// 	double_quote_open = 0;
// 	while (input[i])
// 	{
// 		if (input[i] == '"' && !single_quote_open)
// 			double_quote_open = !double_quote_open;
// 		else if (input[i] == '\'' && !double_quote_open)
// 			single_quote_open = !single_quote_open;
// 		i++;
// 	}
// 	if (single_quote_open || double_quote_open)
// 		return (0);
// 	return (1);
// }

// t_input	init_input(char *input)
// {
// 	t_input	prepare;

// 	prepare.i = 0;
// 	prepare.j = 0;
// 	prepare.quote = '\0';
// 	prepare.len = ft_strlen(input);
// 	prepare.new_len = prepare.len * 2;
// 	prepare.new_input = malloc(prepare.new_len + 1);
// 	if (!prepare.new_input)
// 		prepare.new_input = NULL;
// 	return (prepare);
// }

void	if_input_is_qoutaton(t_input *prepare, char **input)
{
	(*prepare).quote = (*input)[(*prepare).i++];
	(*prepare).new_input[(*prepare).j++] = (*prepare).quote;
	while ((*input)[(*prepare).i] && (*input)[(*prepare).i] != (*prepare).quote)
		(*prepare).new_input[(*prepare).j++] = (*input)[(*prepare).i++];
	if ((*input)[(*prepare).i] == (*prepare).quote)
		(*prepare).new_input[(*prepare).j++] = (*input)[(*prepare).i++];
}

void	proprocess_loop(t_input *prepare, char **input)
{
	if ((*input)[(*prepare).i] == '\'' || (*input)[(*prepare).i] == '"')
		if_input_is_qoutaton(prepare, input);
	else if ((*input)[(*prepare).i] == '|' || (*input)[(*prepare).i] == '<'
		|| (*input)[(*prepare).i] == '>')
	{
		if ((*prepare).i > 0 && (*input)[(*prepare).i - 1] != ' ')
			(*prepare).new_input[(*prepare).j++] = ' ';
		(*prepare).new_input[(*prepare).j++] = (*input)[(*prepare).i];
		if (((*input)[(*prepare).i] == '<' || (*input)[(*prepare).i] == '>')
			&& (*input)[(*prepare).i + 1] == (*input)[(*prepare).i])
		{
			(*prepare).i++;
			(*prepare).new_input[(*prepare).j++] = (*input)[(*prepare).i];
		}
		if ((*input)[(*prepare).i + 1] && (*input)[(*prepare).i + 1] != ' ')
			(*prepare).new_input[(*prepare).j++] = ' ';
		(*prepare).i++;
	}
	else
		(*prepare).new_input[(*prepare).j++] = (*input)[(*prepare).i++];
}

char	*preprocess_input(char *input)
{
	t_input	prepare;

	prepare = init_input(input);
	if (!prepare.new_input)
		return (NULL);
	while (input[prepare.i])
		proprocess_loop(&prepare, &input);
	prepare.new_input[prepare.j] = '\0';
	return (prepare.new_input);
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
				write_error("Minishell:"
					"syntax error near unexpected token `||'\n");
				return (1);
			}
		}
		i++;
	}
	return (0);
}

// void	write_error(char *msg)
// {
// 	write(STDERR_FILENO, msg, ft_strlen(msg));
// }

// void	error_occurd_status(char *msg, int x, t_env_cpy *env)
// {
// 	write_error(msg);
// 	env->last_exit_status = x;
// }

// void	check_main_token(t_token **token, t_env_cpy *env_cpy)
// {
// 	if (check_token(*token))
// 		error_occurd_status("zsh: parse error near `\\n'\n", 2, env_cpy);
// 	else if (search_for_pipe(*token))
// 		execute_pipeline(*token, env_cpy);
// 	else if (search_for_redirection(*token))
// 		check_redirections(token, env_cpy);
// 	else if ((*token)->token_type == CMND)
// 		ft_cmd(*token, env_cpy, 1);
// 	else if ((*token)->token_type == UNKNOWN)
// 	{
// 		ft_putstr_fd((*token)->tokens, 2);
// 		write_error(": Command not found\n");
// 		env_cpy->last_exit_status = 127;
// 	}
// }

// void	if_error(int flag, t_token *token, char *input,
// 		t_env_cpy *env_cpy)
// {
// 	if (flag)
// 	{
// 		free_token_list(token);
// 		env_cpy->last_exit_status = 2;
// 		return ;
// 	}
// 	if (!check_for_quotations(input))
// 	{
// 		error_occurd_status("Syntax error: unmatched quotes\n", 2, env_cpy);
// 		free_token_list(token);
// 		return ;
// 	}
// 	if (is_invalid_pipe_syntax(token))
// 	{
// 		error_occurd_status("Minishell: syntax error near"
// 			" unexpected token `|'\n", 2, env_cpy);
// 		free_token_list(token);
// 		return ;
// 	}
// }

// void	check(char *input, t_env_cpy *env_cpy)
// {
// 	t_token	*token;
// 	int		error_flag;
// 	char	*preprocessed_input;

// 	token = NULL;
// 	error_flag = 0;
// 	if (fix_pipe(input))
// 		return ;
// 	preprocessed_input = preprocess_input(input);
// 	if (!preprocessed_input)
// 	{
// 		error_occurd_status("Error: Memory allocation failed\n", 1, env_cpy);
// 		return ;
// 	}
// 	tokenize_input(preprocessed_input, &token, env_cpy, &error_flag);
// 	free(preprocessed_input);
// 	if (!token)
// 		return ;
// 	if_error(error_flag, token, input, env_cpy);
// 	if (token)
// 		check_main_token(&token, env_cpy);
// 	if (token)
// 		free_token_list(token);
// }
