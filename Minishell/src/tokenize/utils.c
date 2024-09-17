/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:07:00 by mohamibr          #+#    #+#             */
/*   Updated: 2024/09/17 23:56:49 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*ft_strndup(const char *s, size_t n)
{
	char	*dup;

	dup = malloc(n + 1);
	if (!dup)
		return (NULL);
	ft_memcpy(dup, s, n);
	dup[n] = '\0';
	return (dup);
}

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

void	free_env_list(t_env_cpy *head)
{
	t_env_cpy	*tmp;

	while (head)
	{
		tmp = head;
		head = head->next;
		if (tmp->env)
			free(tmp->env);
		if (tmp->type)
			free(tmp->type);
		free(tmp);
	}
}

void	free_single_env_list(t_env_cpy *head)
{
	t_env_cpy	*tmp;

	tmp = head;
	head = head->next;
	if (tmp->env)
		free(tmp->env);
	if (tmp->type)
		free(tmp->type);
	free(tmp);
}
