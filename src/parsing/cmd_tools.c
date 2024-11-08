/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_tools.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mustafa-machlouch <mustafa-machlouch@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 15:24:25 by mmachlou          #+#    #+#             */
/*   Updated: 2024/10/26 14:48:03 by mustafa-mac      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	fill_and_shape_path(char **path_env, char ***paths, t_env_cpy *env)
{
	(*path_env) = get_old_path(env, "PATH");
	if (!(*path_env))
		return (1);
	(*paths) = ft_split((*path_env), ':');
	if (!(*paths))
		return (1);
	return (0);
}

char	*find_in_path(char *cmd, t_env_cpy *env)
{
	char	*path_env;
	char	**paths;
	char	*full_path;
	int		i;

	if (fill_and_shape_path(&path_env, &paths, env))
		return (NULL);
	i = -1;
	while (paths[++i])
	{
		full_path = malloc(strlen(paths[i]) + strlen(cmd) + 2);
		if (!full_path)
			return (NULL);
		strcpy(full_path, paths[i]);
		strcat(full_path, "/");
		strcat(full_path, cmd);
		if (access(full_path, F_OK) == 0)
		{
			ft_free_2darray(paths);
			return (full_path);
		}
		free(full_path);
	}
	ft_free_2darray(paths);
	return (NULL);
}

void	ft_exit(t_token *token, t_env_cpy *env)
{
	int	exit_code;

	exit_code = 0;
	if (token->next)
	{
		if (!ft_is_numeric(token->next->tokens))
		{
			ft_putstr_fd("exit: ", 2);
			ft_putstr_fd(token->next->tokens, 2);
			write_error(": numeric argument required\n");
			exit_code = 2;
		}
		else
		{
			exit_code = ft_atoi(token->next->tokens);
			if (exit_code < 0)
				exit_code = 256 + (exit_code % 256);
			else
				exit_code = exit_code % 256;
		}
	}
	env->last_exit_status = exit_code;
	exit(exit_code);
}
