/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 09:56:49 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/10/22 15:28:18 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	setup_pipeline(t_pipeline *pl, t_token *token_list)
{
	pl->commands = parse_commands(token_list, &pl->num_commands, pl->env_cpy);
	if (!pl->commands)
		return (0);
	pl->num_pipes = pl->num_commands - 1;
	pl->pipes = create_pipes(pl->num_pipes);
	pl->pids = malloc(sizeof(pid_t) * pl->num_commands);
	return (pl->pipes && pl->pids);
}

void	cleanup_pipeline(t_pipeline *pl)
{
	free(pl->pids);
	free_commands(pl->commands);
	free_pipes(pl->pipes, pl->num_pipes);
	if (pl->env_cpy->heredoc_file)
	{
		unlink(pl->env_cpy->heredoc_file);
		free(pl->env_cpy->heredoc_file);
		pl->env_cpy->heredoc_file = NULL;
	}
}

void	close_pipes(int **pipes, int num_pipes)
{
	int	i;

	i = 0;
	while (i < num_pipes)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

void	free_pipes(int **pipes, int num_pipes)
{
	int	i;

	i = 0;
	while (i < num_pipes)
	{
		free(pipes[i]);
		i++;
	}
	free(pipes);
}

void	free_commands(t_command **commands)
{
	int	i;
	int	j;

	i = 0;
	while (commands[i])
	{
		if (commands[i]->infile)
			free(commands[i]->infile);
		if (commands[i]->outfile)
			free(commands[i]->outfile);
		if (commands[i]->argv)
		{
			j = 0;
			while (commands[i]->argv[j])
				free(commands[i]->argv[j++]);
			free(commands[i]->argv);
		}
		if (commands[i]->token_list)
			free_token_list(commands[i]->token_list);
		free(commands[i]);
		i++;
	}
	free(commands);
}
