/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mustafa-machlouch <mustafa-machlouch@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:14:50 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/10/29 11:04:52 by mustafa-mac      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

static	int	count_commands(t_token *token_list)
{
	int	count;

	count = 1;
	while (token_list)
	{
		if (token_list->token_type == PIPE)
			count++;
		token_list = token_list->next;
	}
	return (count);
}

static	t_command	*allocate_command(void)
{
	t_command	*command;

	command = malloc(sizeof(t_command));
	if (!command)
		return (NULL);
	command->infile = NULL;
	command->outfile = NULL;
	command->append = 0;
	command->token_list = NULL;
	command->argv = NULL;
	return (command);
}

static	void	parse_command_tokens(t_token **current,
		t_command *command, t_env_cpy *env_cpy)
{
	while (*current && (*current)->token_type != PIPE)
	{
		if ((*current)->token_type == REDIRECT_IN)
		{
			*current = (*current)->next;
			if (*current && (*current)->tokens)
				command->infile = ft_strdup((*current)->tokens);
		}
		else if ((*current)->token_type == REDIRECT_OUT
			|| (*current)->token_type == APPEND)
		{
			command->append = ((*current)->token_type == APPEND);
			*current = (*current)->next;
			if (*current && (*current)->tokens)
				command->outfile = ft_strdup((*current)->tokens);
		}
		else
			add_token(&command->token_list,
				(*current)->tokens, env_cpy, (*current)->qoute_type);
		*current = (*current)->next;
	}
}

t_command	**parse_commands(t_token *token_list,
	int *num_commands, t_env_cpy *env_cpy)
{
	t_command	**commands;
	t_token		*current;
	int			count;
	int			i;

	count = count_commands(token_list);
	*num_commands = count;
	current = token_list;
	i = 0;
	commands = malloc(sizeof(t_command *) * (count + 1));
	if (!commands)
		return (NULL);
	while (i < count)
	{
		commands[i] = allocate_command();
		if (!commands[i])
			return (free_commands(commands), NULL);
		parse_command_tokens(&current, commands[i], env_cpy);
		commands[i]->argv = allocate_arguments(commands[i]->token_list);
		if (current && current->token_type == PIPE)
			current = current->next;
		i++;
	}
	commands[i] = NULL;
	return (commands);
}
