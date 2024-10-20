/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 15:24:25 by mmachlou          #+#    #+#             */
/*   Updated: 2024/10/20 11:23:37 by mohamibr         ###   ########.fr       */
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
		if (access(full_path, X_OK) == 0)
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
			fprintf(stderr, "exit: %s: numeric argument required\n",
				token->next->tokens);
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

int	prepare_to_exit(t_token *token, t_env_cpy *env_cpy, int shell)
{
	if (token->next && token->next->next)
	{
		env_cpy->last_exit_status = 1;
		fprintf(stderr, "Minishell: pwd: : invalid optioz\n");
		return (1);
	}
	if (shell)
		printf("exit\n");
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
