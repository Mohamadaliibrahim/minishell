/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checking_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 08:57:57 by mohamibr          #+#    #+#             */
/*   Updated: 2024/10/23 08:57:58 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

void	write_error(char *msg)
{
	write(STDERR_FILENO, msg, ft_strlen(msg));
}

void	error_occurd_status(char *msg, int x, t_env_cpy *env)
{
	write_error(msg);
	env->last_exit_status = x;
}

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

int	pipe_or_redirection(char *token)
{
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

int	is_build_ins(char *token)
{
	if ((ft_strcmp(token, "cd") == 0) || (ft_strcmp(token, "export") == 0)
		|| (ft_strcmp(token, "unset") == 0) || (ft_strcmp(token, "exit") == 0)
		|| (ft_strcmp(token, "env") == 0) || (ft_strcmp(token, "echo") == 0)
		|| (ft_strcmp(token, "pwd") == 0))
		return (1);
	return (0);
}
//stop!!
