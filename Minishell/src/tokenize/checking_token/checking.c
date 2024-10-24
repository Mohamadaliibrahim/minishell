/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checking.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 08:57:49 by mohamibr          #+#    #+#             */
/*   Updated: 2024/10/23 08:57:50 by mohamibr         ###   ########.fr       */
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
		error_occurd_status("Error: Memory allocation failed\n", 1, env_cpy);
		return ;
	}
	tokenize_input(preprocessed_input, &token, env_cpy, &error_flag);
	free(preprocessed_input);
	if (!token)
		return ;
	if_error(error_flag, token, input, env_cpy);
	if (token)
		check_main_token(&token, env_cpy);
	if (token)
		free_token_list(token);
}
//stop!!
