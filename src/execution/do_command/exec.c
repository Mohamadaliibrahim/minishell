/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mustafa-machlouch <mustafa-machlouch@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 10:49:47 by mohamibr          #+#    #+#             */
/*   Updated: 2024/10/29 11:04:38 by mustafa-mac      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

static	int	is_directory_error(char **av, t_env_cpy *env_cpy, char **env)
{
	char	*home;
	char	*pwd;

	home = get_env_value("HOME", env_cpy);
	pwd = get_env_value("PWD", env_cpy);
	if ((home && ft_strcmp(av[0], home) == 0)
		|| (pwd && ft_strcmp(av[0], pwd) == 0))
	{
		write_error("minishell: ");
		write_error(av[0]);
		write_error(": is a directory\n");
		env_cpy->last_exit_status = 126;
		ft_free_2darray(av);
		ft_free_2darray(env);
		return (1);
	}
	return (0);
}

static void	cleanup_env_av(char *cmd_path, char **env,
	char **av, t_env_cpy *env_cpy)
{
	free(cmd_path);
	ft_free_2darray(env);
	ft_free_2darray(av);
	if (env_cpy->heredoc_file)
	{
		unlink(env_cpy->heredoc_file);
		free(env_cpy->heredoc_file);
		env_cpy->heredoc_file = NULL;
	}
}

void	do_comand(t_token *token, t_env_cpy *env_cpy)
{
	char	**env;
	char	**av;
	char	*cmd_path;
	pid_t	pid;

	env = list_to_2d(env_cpy);
	av = allocate_arguments(token);
	if (!av)
		return (ft_free_2darray(env));
	if (is_directory_error(av, env_cpy, env))
		return ;
	cmd_path = get_command_path(av, env_cpy);
	if (!cmd_path)
		return (cleanup_env_av(NULL, env, av, env_cpy));
	pid = fork();
	if (pid == 0)
		execute_command(cmd_path, av, env, env_cpy);
	else if (pid > 0)
		handle_parent_process(pid, env_cpy);
	else
		env_cpy->last_exit_status = (perror("fork"), 1);
	cleanup_env_av(cmd_path, env, av, env_cpy);
}
