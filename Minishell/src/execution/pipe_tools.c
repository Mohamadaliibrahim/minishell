/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:14:50 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/10/22 15:28:13 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static	void	setup_input_redirection(t_pipeline *pl, int i)
{
	int		infile_fd;
	char	*input_file;

	if (pl->commands[i]->infile)
		input_file = pl->commands[i]->infile;
	else
		input_file = pl->env_cpy->heredoc_file;
	if (input_file)
	{
		infile_fd = open(input_file, O_RDONLY);
		if (infile_fd < 0)
		{
			perror(input_file);
			cleanup_pipeline(pl);
			exit(1);
		}
		dup2(infile_fd, STDIN_FILENO);
		close(infile_fd);
	}
	else if (i > 0)
		dup2(pl->pipes[i - 1][0], STDIN_FILENO);
}

static	void	setup_output_redirection(t_pipeline *pl, int i)
{
	int	outfile_fd;
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (pl->commands[i]->append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	if (pl->commands[i]->outfile)
	{
		outfile_fd = open(pl->commands[i]->outfile, flags, 0644);
		if (outfile_fd < 0)
		{
			perror(pl->commands[i]->outfile);
			cleanup_pipeline(pl);
			exit(EXIT_FAILURE);
		}
		dup2(outfile_fd, STDOUT_FILENO);
		close(outfile_fd);
	}
	else if (i < pl->num_commands - 1)
		dup2(pl->pipes[i][1], STDOUT_FILENO);
}

void	execute_child_process(t_pipeline *pl, int i)
{
	setup_input_redirection(pl, i);
	setup_output_redirection(pl, i);
	close_pipes(pl->pipes, pl->num_pipes);
	ft_cmd(pl->commands[i]->token_list, pl->env_cpy, 0);
	exit(pl->env_cpy->last_exit_status);
}
