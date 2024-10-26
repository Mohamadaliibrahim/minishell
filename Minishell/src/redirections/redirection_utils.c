/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mustafa-machlouch <mustafa-machlouch@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 14:31:24 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/10/26 14:31:25 by mustafa-mac      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	search_for_redirection(t_token *head)
{
	t_token	*token;

	token = head;
	while (token)
	{
		if (token->token_type == REDIRECT_OUT || token->token_type == APPEND
			|| token->token_type == REDIRECT_IN)
		{
			return (1);
		}
		token = token->next;
	}
	return (0);
}

char	*get_filename(t_token *token)
{
	while (token)
	{
		if ((token->token_type == APPEND || token->token_type == REDIRECT_OUT
				|| token->token_type == REDIRECT_IN))
		{
			if (token->next == NULL)
				return (NULL);
			return (token->next->tokens);
		}
		token = token->next;
	}
	return (NULL);
}

void	dup2_error(int *out, int *in)
{
	dup2((*out), STDOUT_FILENO);
	close(*out);
	dup2((*in), STDIN_FILENO);
	close(*in);
}

void	error_occured(int *out, int *in, t_env_cpy *env)
{
	perror("dup2");
	close(*out);
	close(*in);
	env->last_exit_status = 1;
}

int	its_redirection(t_token *token)
{
	if (token->next && ((token->next->token_type == REDIRECT_IN)
			|| (token->next->token_type == REDIRECT_OUT)
			|| (token->next->token_type == APPEND)
			|| (token->next->token_type == HEREDOC)
			|| (token->next->token_type == PIPE)))
		return (1);
	return (0);
}
//stop!