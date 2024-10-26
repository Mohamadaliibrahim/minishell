/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 10:49:47 by mohamibr          #+#    #+#             */
/*   Updated: 2024/10/24 10:41:27 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static	void	setup_heredoc(t_env_cpy *env_cpy)
{
	int	infile_fd;

	if (env_cpy->heredoc_file)
	{
		infile_fd = open(env_cpy->heredoc_file, O_RDONLY);
		if (infile_fd < 0)
		{
			perror("minishell: open heredoc file");
			exit(EXIT_FAILURE);
		}
		dup2(infile_fd, STDIN_FILENO);
		close(infile_fd);
	}
}

static void	handle_execve_error(char *cmd_path, char **av, char **env)
{
	perror("Execution error");
	ft_free_2darray(env);
	ft_free_2darray(av);
	free(cmd_path);
	if (errno == EACCES || errno == EISDIR || errno == ENOEXEC
		|| errno == EINVAL || errno == ETXTBSY)
		exit(126);
	else
		exit(EXIT_FAILURE);
}

void	execute_command(char *cmd_path, char **av,
	char **env, t_env_cpy *env_cpy)
{
	setup_heredoc(env_cpy);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (execve(cmd_path, av, env) == -1)
		handle_execve_error(cmd_path, av, env);
}
