/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_checking.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mustafa-machlouch <mustafa-machlouch@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 14:31:30 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/10/26 14:31:30 by mustafa-mac      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	check_token(t_token *head)
{
	t_token	*token;

	token = head;
	while (token)
	{
		if (token->token_type == PIPE && (token->next == NULL
				|| token->next->token_type == PIPE))
			return (1);
		else if (token && ((token->token_type == REDIRECT_IN)
				|| (token->token_type == REDIRECT_OUT)
				|| (token->token_type == APPEND)
				|| (token->token_type == HEREDOC)
				|| (token->token_type == PIPE)))
		{
			if (!token->next)
				return (1);
			else if (its_redirection(token))
				return (1);
		}
		token = token->next;
	}
	return (0);
}

// tokenize_check.c

void	check_redirections(t_token **token, t_env_cpy *env)
{
	if (*token && ((*token)->token_type == REDIRECT_IN
			|| (*token)->token_type == REDIRECT_OUT
			|| (*token)->token_type == APPEND
			|| (*token)->token_type == HEREDOC))
	{
		if ((*token)->next && ((*token)->next->token_type == REDIRECT_IN
				|| (*token)->next->token_type == REDIRECT_OUT
				|| (*token)->next->token_type == APPEND
				|| (*token)->next->token_type == HEREDOC))
		{
			perror((*token)->next->tokens);
			env->last_exit_status = 2;
			return ;
		}
	}
	if (check_token(*token))
	{
		perror((*token)->tokens);
		env->last_exit_status = 2;
		return ;
	}
	ft_redirection(token, env);
}

int	checking_for_errors(t_env_cpy *env, int *in, int *out)
{
	if (env->last_exit_status != 0)
	{
		dup2_error(out, in);
		return (1);
	}
	if (env->last_input_fd != -1)
	{
		if (dup2(env->last_input_fd, STDIN_FILENO) == -1)
		{
			error_occured(out, in, env);
			return (1);
		}
		close(env->last_input_fd);
	}
	if (env->last_output_fd != -1)
	{
		if (dup2(env->last_output_fd, STDOUT_FILENO) == -1)
		{
			error_occured(out, in, env);
			return (1);
		}
		close(env->last_output_fd);
	}
	return (0);
}
