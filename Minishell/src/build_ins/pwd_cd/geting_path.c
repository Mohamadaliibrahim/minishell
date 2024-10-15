/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   geting_path.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 07:45:28 by mohamibr          #+#    #+#             */
/*   Updated: 2024/10/15 09:02:45 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

char	*get_cd_path_helper(t_token **token)
{
	char	*path;

	if (ft_strcmp((*token)->next->tokens, ".") == 0)
		path = ".";
	else if (ft_strcmp((*token)->next->tokens, "..") == 0)
		path = "..";
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

char	*if_cd_with_dash(t_env_cpy *env_cpy, int *should_free)
{
	char	*path;

	path = get_oldpwd(env_cpy);
	if (!path)
	{
		ft_putstr_fd("cd: OLDPWD not set\n", 2);
		env_cpy->last_exit_status = 1;
		return (NULL);
	}
	*should_free = 1;
	return (path);
}

// hay el function btrj3lk el path lal token

char	*get_cd_path(t_token *token, t_env_cpy *env_cpy, int *should_free)
{
	char	*path;

	*should_free = 0;
	if (token->next == NULL || ft_strcmp(token->next->tokens, "~") == 0)
		path = if_just_cd(env_cpy, should_free);
	else if (ft_strcmp(token->next->tokens, "-") == 0)
		path = if_cd_with_dash(env_cpy, should_free);
	else if ((ft_strcmp(token->next->tokens, ".") == 0)
		|| (ft_strcmp(token->next->tokens, "..") == 0))
		path = get_cd_path_helper(&token);
	else
	{
		if (token->next->tokens[0] == '-' && ft_strlen(token->next->tokens) > 1)
		{
			ft_putstr_fd("cd: invalid option\n", 2);
			env_cpy->last_exit_status = 1;
			return (NULL);
		}
		path = token->next->tokens;
	}
	return (path);
}
