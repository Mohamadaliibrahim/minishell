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

char	*get_command_path(char **av, t_env_cpy *env_cpy)
{
	char	*cmd_path;

	if (ft_strncmp(av[0], "/", 1) == 0 || ft_strncmp(av[0], "./", 2) == 0
		|| ft_strncmp(av[0], "../", 3) == 0)
		cmd_path = ft_strdup(av[0]);
	else
		cmd_path = find_in_path(av[0], env_cpy);
	if (!cmd_path)
	{
		write_error(av[0]);
		write_error(": command not found\n");
		env_cpy->last_exit_status = 127;
	}
	return (cmd_path);
}

static	void	update_exit_status(int status, t_env_cpy *env_cpy)
{
	int	sig;

	if (WIFEXITED(status))
		env_cpy->last_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		env_cpy->last_exit_status = 128 + sig;
		if (sig == SIGINT)
			write(1, "\n", 1);
		else if (sig == SIGQUIT)
			write(1, "Quit: 3\n", 8);
	}
	else
		env_cpy->last_exit_status = 0;
}

void	handle_parent_process(int pid, t_env_cpy *env_cpy)
{
	int		status;
	void	(*prev_sigint_handler)(int);

	prev_sigint_handler = signal(SIGINT, SIG_IGN);
	if (waitpid(pid, &status, 0) == -1)
	{
		perror("waitpid");
		env_cpy->last_exit_status = 1;
	}
	else
	{
		signal(SIGINT, prev_sigint_handler);
		update_exit_status(status, env_cpy);
	}
}
