/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getting_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 07:45:28 by mohamibr          #+#    #+#             */
/*   Updated: 2024/11/05 21:54:13 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

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

void	error_statment(t_env_cpy *env_cpy, int x)
{
	env_cpy->last_exit_status = x;
	ft_putstr_fd("cd: invalid option\n", 2);
}

void	no_idea_what_to_name(t_env_cpy *env_cpy, char *msg, int x)
{
	perror(msg);
	env_cpy->last_exit_status = x;
}

char	*expand_tilde(char *path, t_env_cpy *env_cpy, int *should_free)
{
	char	*home_dir;
	char	*expanded_path;

	if (path && path[0] == '~')
	{
		home_dir = get_env_msg(env_cpy, "HOME");
		if (!home_dir)
		{
			no_idea_what_to_name(env_cpy, "cd: HOME not set\n", 1);
			return (NULL);
		}
		expanded_path = ft_strjoin(home_dir, path + 1);
		if (!expanded_path)
		{
			no_idea_what_to_name(env_cpy, "Memory allocation error", 1);
			free(home_dir);
			return (NULL);
		}
		free(home_dir);
		if (*should_free)
			free(path);
		path = expanded_path;
		*should_free = 1;
	}
	return (path);
}

char	*determine_initial_path(t_token *token, t_env_cpy *env_cpy,
	int *should_free)
{
	char	*path;

	if (token->next == NULL || (ft_strcmp(token->next->tokens, "~") == 0)
		|| (ft_strcmp(token->next->tokens, "--") == 0))
		path = if_just_cd(env_cpy, should_free);
	else if (ft_strncmp(token->next->tokens, "---", 3) == 0)
	{
		error_statment(env_cpy, 2);
		return (NULL);
	}
	else if (ft_strcmp(token->next->tokens, "-") == 0)
	{
		if (if_dash(env_cpy, should_free, &path))
			return (NULL);
	}
	else if (dot(&token))
		path = get_cd_path_helper(&token);
	else
	{
		if (else_else(token, env_cpy, &path))
			return (NULL);
	}
	return (path);
}

char	*get_cd_path(t_token *token, t_env_cpy *env_cpy, int *should_free)
{
	char	*path;

	*should_free = 0;
	path = determine_initial_path(token, env_cpy, should_free);
	if (!path)
		return (NULL);
	path = expand_tilde(path, env_cpy, should_free);
	return (path);
}

// char *get_cd_path(t_token *token, t_env_cpy *env_cpy, int *should_free)
// {
// 	char *path;
// 	char *home_dir;
// 	char *expanded_path;

// 	*should_free = 0;
// 	if (token->next == NULL || (ft_strcmp(token->next->tokens, "~") == 0)
// 		|| (ft_strcmp(token->next->tokens, "--") == 0))
// 		path = if_just_cd(env_cpy, should_free);
// 	else if (ft_strncmp(token->next->tokens, "---", 3) == 0)
// 	{
// 		error_statment(env_cpy, 2);
// 		return (NULL);
// 	}
// 	else if (ft_strcmp(token->next->tokens, "-") == 0)
// 	{
// 		if (if_dash(env_cpy, should_free, &path))
// 			return (NULL);
// 	}
// 	else if (dot(&token))
// 		path = get_cd_path_helper(&token);
// 	else
// 	{
// 		if (else_else(token, env_cpy, &path))
// 			return (NULL);
// 	}
// 	if (path && path[0] == '~')
// 	{
// 		home_dir = get_env_msg(env_cpy, "HOME");
// 		if (!home_dir)
// 		{
// 			ft_putstr_fd("cd: HOME not set\n", 2);
// 			env_cpy->last_exit_status = 1;
// 			return (NULL);
// 		}
// 		expanded_path = ft_strjoin(home_dir, path + 1);
// 		if (!expanded_path)
// 		{
// 			perror("Memory allocation error");
// 			env_cpy->last_exit_status = 1;
// 			free(home_dir);
// 			return (NULL);
// 		}
// 		free(home_dir);
// 		if (*should_free)
// 			free(path);
// 		path = expanded_path;
// 		*should_free = 1;
// 	}
// 	return (path);
// }
