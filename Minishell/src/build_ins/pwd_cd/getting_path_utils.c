/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getting_path_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 07:45:28 by mohamibr          #+#    #+#             */
/*   Updated: 2024/11/05 21:54:27 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

int	if_dash(t_env_cpy *env_cpy, int	*should_free, char **path)
{
	if (env_cpy->internal_oldpwd)
	{
		(*path) = ft_strdup(env_cpy->internal_oldpwd);
		*should_free = 1;
	}
	else
	{
		ft_putstr_fd("cd: OLDPWD not set\n", 2);
		env_cpy->last_exit_status = 1;
		return (1);
	}
	return (0);
}

int	else_else(t_token *token, t_env_cpy *env_cpy, char **path)
{
	if (token->next->tokens[0] == '-' && ft_strlen(token->next->tokens) > 1)
	{
		error_statment(env_cpy, 1);
		return (1);
	}
	(*path) = token->next->tokens;
	return (0);
}

int	checking(t_cd cd, t_env_cpy *env_cpy)
{
	if (!cd.path)
	{
		freeing_cd(cd, env_cpy, env_cpy->last_exit_status);
		return (1);
	}
	if (!cd.old_pwd)
		old_pwd_is_null(cd, env_cpy);
	if (chdir(cd.path) == -1)
	{
		write_error("minishell: cd: ");
		write_error(cd.path);
		write_error(": ");
		write_error(strerror(errno));
		write_error("\n");
		freeing_cd(cd, env_cpy, 1);
		return (1);
	}
	return (0);
}

char	*get_cd_path_helper(t_token **token)
{
	char	*path;

	if (ft_strcmp((*token)->next->tokens, ".") == 0)
		path = ".";
	else if (ft_strcmp((*token)->next->tokens, "..") == 0)
		path = "..";
	else
		path = (*token)->next->tokens;
	return (path);
}

char	*if_just_cd(t_env_cpy *env_cpy, int *should_free)
{
	char	*path;

	path = get_env_msg(env_cpy, "HOME");
	if (!path)
	{
		ft_putstr_fd("cd: HOME not set\n", 2);
		env_cpy->last_exit_status = 1;
		return (NULL);
	}
	*should_free = 1;
	return (path);
}
//stop!