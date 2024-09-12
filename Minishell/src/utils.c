/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:07:00 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/09/12 12:29:01 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	ft_free_2darray(char **tokens)
{
	int	i;

	i = 0;
	if (!tokens)
		return ;
	while (tokens[i] != NULL)
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}

char	*find_in_path(char *cmd)
{
	char	*path_env;
	char	**paths;
	char	*full_path;
	int		i;

	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
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

void	free_token_list(t_token *head)
{
	t_token	*tmp;

	while (head)
	{
		tmp = head;
		head = head->next;
		free(tmp->tokens);
		free(tmp);
	}
}

int	check_type(char *token)
{
	char	*cmd_path;

	if (ft_strncmp(token, "/", 1) == 0
		|| ft_strncmp(token, "./", 2) == 0
		|| ft_strncmp(token, "../", 3) == 0)
	{
		if (access(token, X_OK) == 0)
			return (CMND);
		else
			return (UNKNOWN);
	}
	cmd_path = find_in_path(token);
	if (cmd_path != NULL)
	{
		free(cmd_path);
		return (CMND);
	}
	else if (ft_strcmp(token, "|") == 0)
		return (PIPE);
	else if (ft_strcmp(token, "<") == 0
		|| ft_strcmp(token, ">") == 0
		|| ft_strcmp(token, ">>") == 0)
		return (REDIRECT_OUT);
	else if (ft_strcmp(token, "&") == 0)
		return (UNKNOWN);
	else if (ft_strncmp(token, "$", 1) == 0)
		return (VARIABLE);
	else if (ft_strcmp(token, "'") == 0 || ft_strcmp(token, "\"") == 0)
		return (QUOTE);
	else if (ft_strcmp(token, ";") == 0)
		return (UNKNOWN);
	else if (ft_strcmp(token, "<<") == 0)
		return (HEREDOC);
	return (UNKNOWN);
}
