/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checking.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 08:57:49 by mohamibr          #+#    #+#             */
/*   Updated: 2024/10/24 10:31:48 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

int	check_for_dollar(char *token, t_env_cpy *env, char **cmd_path,
		char **expanded_token)
{
	if (token[0] == '$' && ft_strlen(token) > 1)
	{
		(*expanded_token) = get_env_value(token + 1, env);
		if (*expanded_token)
		{
			(*cmd_path) = find_in_path(*expanded_token, env);
			if (*cmd_path != NULL)
			{
				free(*cmd_path);
				return (CMND);
			}
		}
		return (UNKNOWN);
	}
	return (-1);
}

int	check_type(char *token, t_env_cpy *env, char qoute)
{
	char	*cmd_path;
	char	*expanded_token;

	if (qoute != 0)
		return (CMND);
	if (ft_strncmp(token, "/", 1) == 0 || ft_strncmp(token, "./", 2) == 0
		|| ft_strncmp(token, "../", 3) == 0)
	{
		if (access(token, F_OK) == 0)
			return (CMND);
		else
			return (UNKNOWN);
	}
	if (check_for_dollar(token, env, &cmd_path, &expanded_token) != -1)
		return (check_for_dollar(token, env, &cmd_path, &expanded_token));
	if (is_build_ins(token))
		return (CMND);
	cmd_path = find_in_path(token, env);
	if (cmd_path != NULL)
	{
		free(cmd_path);
		return (CMND);
	}
	return (pipe_or_redirection(token));
}

void	check_main_token(t_token **token, t_env_cpy *env_cpy)
{
	if (check_token(*token))
		error_occurd_status("zsh: parse error near `\\n'\n", 2, env_cpy);
	else if (search_for_pipe(*token))
		execute_pipeline(*token, env_cpy);
	else if (search_for_redirection(*token))
		check_redirections(token, env_cpy);
	else if ((*token)->token_type == CMND)
		ft_cmd(*token, env_cpy, 1);
	else if ((*token)->token_type == UNKNOWN)
	{
		ft_putstr_fd((*token)->tokens, 2);
		write_error(": Command not found\n");
		env_cpy->last_exit_status = 127;
	}
}

int	check_pipe(t_token *head, t_env_cpy *env)
{
	if (head->token_type == PIPE)
	{
		write_error("Minishell: syntax error near unexpected token `|'\n");
		env->last_exit_status = 2;
		free_token_list(head);
		return (1);
	}
	return (0);
}

void	check(char *input, t_env_cpy *env_cpy)
{
	t_token	*token;
	int		error_flag;
	char	*preprocessed_input;

	if (fix_pipe(input, env_cpy))
		return ;
	token = NULL;
	error_flag = 0;
	preprocessed_input = preprocess_input(input);
	if (!preprocessed_input)
	{
		error_occurd_status("Error: Memory allocation failed\n", 1, env_cpy);
		return ;
	}
	tokenize_input(preprocessed_input, &token, env_cpy, &error_flag);
	free(preprocessed_input);
	if (!token || check_pipe(token, env_cpy))
		return ;
	if (if_error(error_flag, token, input, env_cpy))
		return ;
	if (token)
		check_main_token(&token, env_cpy);
	if (token)
		free_token_list(token);
}
//stop!!
