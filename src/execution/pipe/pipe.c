/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mustafa-machlouch <mustafa-machlouch@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:14:50 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/10/29 11:05:03 by mustafa-mac      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

int	**create_pipes(int num_pipes)
{
	int	**pipes;
	int	i;

	pipes = malloc(sizeof(int *) * num_pipes);
	i = 0;
	if (!pipes)
		return (NULL);
	while (i < num_pipes)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i] || pipe(pipes[i]) == -1)
		{
			perror("pipe");
			free_pipes(pipes, i + (pipes[i] != NULL));
			if (pipes[i] != NULL)
				free(pipes[i]);
			return (NULL);
		}
		i++;
	}
	return (pipes);
}

static	void	wait_for_children(t_pipeline *pl)
{
	int	status;
	int	i;
	int	sig;

	i = 0;
	while (i < pl->num_commands)
	{
		waitpid(pl->pids[i], &status, 0);
		if (WIFEXITED(status))
			pl->env_cpy->last_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			sig = WTERMSIG(status);
			pl->env_cpy->last_exit_status = 128 + sig;
			if (sig == SIGINT)
				write(1, "\n", 1);
			else if (sig == SIGQUIT)
				write(1, "Quit: 3\n", 8);
		}
		i++;
	}
}

static	int	handle_fork(t_pipeline *pl, int i)
{
	pl->pids[i] = fork();
	if (pl->pids[i] == -1)
	{
		perror("fork");
		pl->env_cpy->last_exit_status = 1;
		cleanup_pipeline(pl);
		return (0);
	}
	if (pl->pids[i] == 0)
		execute_child_process(pl, i);
	return (1);
}

static	void	close_pipeline_pipes(t_pipeline *pl, int i)
{
	if (i > 0)
		close(pl->pipes[i - 1][0]);
	if (i < pl->num_commands - 1)
		close(pl->pipes[i][1]);
}

void	execute_pipeline(t_token *token_list, t_env_cpy *env_cpy)
{
	t_pipeline	pl;
	int			i;

	pl = (t_pipeline){.env_cpy = env_cpy};
	i = 0;
	if (!setup_pipeline(&pl, token_list))
		return ;
	while (i < pl.num_commands)
	{
		if (!handle_fork(&pl, i))
			return ;
		close_pipeline_pipes(&pl, i);
		i++;
	}
	close_pipes(pl.pipes, pl.num_pipes);
	wait_for_children(&pl);
	cleanup_pipeline(&pl);
}
