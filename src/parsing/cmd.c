/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 15:24:25 by mmachlou          #+#    #+#             */
/*   Updated: 2024/12/26 11:11:39 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	prepare_to_exit(t_token *token, t_env_cpy *env_cpy, int shell)
{
	if (token->next && token->next->next)
	{
		write_error("Minishell: pwd: : invalid option\n");
		env_cpy->last_exit_status = 1;
		return (1);
	}
	if (shell)
		ft_printf("exit\n");
	ft_exit(token, env_cpy);
	return (0);
}

void	build_ins(t_token *token, t_env_cpy *env_cpy, int is_main_shell)
{
	if ((ft_strcmp(token->tokens, "echo") == 0))
		check_echo(token, env_cpy);
	else if ((ft_strcmp(token->tokens, "pwd") == 0))
	{
		env_cpy->last_exit_status = 0;
		ft_pwd(token, env_cpy);
	}
	else if ((ft_strcmp(token->tokens, "env") == 0))
		ft_env(token, env_cpy);
	else if ((ft_strcmp(token->tokens, "export") == 0))
		ft_export(token, env_cpy);
	else if ((ft_strcmp(token->tokens, "unset") == 0))
		ft_unset(token, &env_cpy);
	else if ((ft_strcmp(token->tokens, "cd") == 0))
	{
		env_cpy->last_exit_status = 0;
		ft_cd(token, env_cpy);
	}
	else if ((ft_strcmp(token->tokens, "exit") == 0))
		prepare_to_exit(token, env_cpy, is_main_shell);
	else
		do_comand(token, env_cpy);
}

void	ft_cmd(t_token *token, t_env_cpy *env_cpy, int is_main_shell)
{
	char	*expanded_cmd;

	if (token == NULL)
		return ;
	if (token->tokens[0] == '$')
	{
		expanded_cmd = get_env_value(token->tokens + 1, env_cpy);
		if (expanded_cmd)
		{
			free(token->tokens);
			token->tokens = ft_strdup(expanded_cmd);
		}
	}
	build_ins(token, env_cpy, is_main_shell);
}
