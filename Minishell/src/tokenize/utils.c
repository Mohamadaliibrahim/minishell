/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:07:00 by mohamibr          #+#    #+#             */
/*   Updated: 2024/10/15 06:50:16 by mohamibr         ###   ########.fr       */
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

void	free_token_list(t_token *token_list)
{
	t_token	*current;
	t_token	*next;

	current = token_list;
	while (current)
	{
		next = current->next;
		if (current->tokens)
			free(current->tokens);
		free(current);
		current = next;
	}
}

void	free_env_list(t_env_cpy *head)
{
	t_env_cpy	*tmp;

	while (head)
	{
		tmp = head;
		head = head->next;
		free(tmp->env);
		free(tmp->type);
		if (tmp->internal_pwd)
			free(tmp->internal_pwd);
		if (tmp->internal_oldpwd)
			free(tmp->internal_oldpwd);
		free(tmp);
	}
}



char *ft_strjoin_free(char *s1, char *s2)
{
    char *new_str;

    if (!s1 || !s2)
        return NULL;
    new_str = ft_strjoin(s1, s2);
    if (!new_str)
        return NULL;
    free(s1);
    return new_str;
}