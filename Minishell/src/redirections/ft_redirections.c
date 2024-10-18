/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirections.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 15:57:49 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/10/18 18:59:17 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// ft_redirections.c
int	save_backups(int *out, int *in, t_env_cpy *env)
{
	*out = dup(STDOUT_FILENO);
	if (*out == -1)
	{
		perror("dup");
		env->last_exit_status = 1;
		return (1);
	}
	*in = dup(STDIN_FILENO);
	if (*in == -1)
	{
		perror("dup");
		close(*out);
		env->last_exit_status = 1;
		return (1);
	}
	return (0);
}

void	redirection_head_loop(t_token **head, t_env_cpy *env)
{
	while (*head)
	{
		if ((*head)->token_type == REDIRECT_OUT)
			ft_trunck(*head, env);
		else if ((*head)->token_type == APPEND)
			ft_append(*head, env);
		else if ((*head)->token_type == REDIRECT_IN)
			ft_infile(*head, env);
		*head = (*head)->next;
	}
}

int	main_process_redirection(int *out, int *in, t_env_cpy *env, t_token **token)
{
	t_token	*head;

	head = *token;
	if (save_backups(&(*out), &(*in), env))
		return (1);
	redirection_head_loop(&head, env);
	remove_redirection_tokens(token);
	if (checking_for_errors(env, &(*in), &(*out)))
		return (1);
	return (0);
}

void	ft_redirection(t_token **token, t_env_cpy *env)
{
	int	stdout_backup;
	int	stdin_backup;

	env->last_output_fd = -1;
	env->last_input_fd = -1;
	env->last_exit_status = 0;
	if (main_process_redirection(&stdout_backup, &stdin_backup, env, token))
		return ;
	if (env->last_exit_status == 0)
		ft_cmd(*token, env, 1);
	if (dup2(stdout_backup, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		env->last_exit_status = 1;
	}
	close(stdout_backup);
	if (dup2(stdin_backup, STDIN_FILENO) == -1)
	{
		perror("dup2");
		env->last_exit_status = 1;
	}
	close(stdin_backup);
}
