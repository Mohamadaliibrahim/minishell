/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fix_pipe.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 15:48:43 by mohamibr          #+#    #+#             */
/*   Updated: 2024/12/26 10:49:59 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

int	its(char *str, int i, t_env_cpy *env)
{
	if ((str[i + 1] == '"' || str[i + 1] == '\'') && str[i + 2] == '\0')
	{
		env->last_exit_status = 2;
		write_error("Minishell:""syntax error near unexpected token `|'\n");
		return (1);
	}
	if ((str[i + 1] == '"' || str[i + 1] == '\'')
		&& (str[i + 2] == '"' || str[i + 2] == '\''))
	{
		env->last_exit_status = 2;
		write_error("Minishell:""syntax error near unexpected token `|'\n");
		return (1);
	}
	return (0);
}

int	else_if_pipe(t_env_cpy *env, char *str, int i)
{
	if (str[i + 1] == '|')
	{
		env->last_exit_status = 2;
		write_error("Minishell: syntax error near unexpected token `||'\n");
		return (1);
	}
	else if (str[i + 1] == '\0' || (str[i + 1] == ' ' && str[i + 2] == '\0'))
	{
		env->last_exit_status = 2;
		write_error("Minishell: syntax error near unexpected token `|'\n");
		return (1);
	}
	else if (its(str, i, env))
		return (1);
	return (0);
}

int	fix_pipe(char *str, t_env_cpy *env)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] == '$')
			return (1);
		if (str[i] == '"' || str[i] == '\'')
		{
			if (quote == 0)
				quote = str[i];
			else if (quote == str[i])
				quote = 0;
		}
		else if (str[i] == '|' && quote == 0)
		{
			if (else_if_pipe(env, str, i))
				return (1);
		}
		i++;
	}
	return (0);
}
