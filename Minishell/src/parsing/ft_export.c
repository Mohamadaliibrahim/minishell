/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 15:13:03 by mohamibr          #+#    #+#             */
/*   Updated: 2024/09/15 20:38:28 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	print_export(t_env_cpy *env_cpy)
{
	if (env_cpy->next)
	{
		if (env_cpy->equal == true)
		{
			printf("declare -x ");
			printf("%s", env_cpy->type);
			printf("=\"");
			printf("%s\"\n", env_cpy->env);
		}
		else
		{
			printf("declare -x ");
			printf("%s", env_cpy->type);
		}
	}
	else
	{
		printf("%s\"\n", env_cpy->env);
	}
}

void	add_env_cpy(t_env_cpy **head)
{
	t_env_cpy	*new_node;
	t_env_cpy	*tmp;

	new_node = malloc(sizeof(t_env_cpy));
	if (!new_node)
		return ;
	new_node->type = NULL;
	new_node->env = NULL;
	new_node->equal = false;
	new_node->next = NULL;
	if (!*head)
		*head = new_node;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_node;
	}
}

t_env_cpy	*fill_token(t_env_cpy *env_cpy, char *str)
{
	add_env_cpy(&env_cpy);
	env_cpy->equal = check_for_equal(str);
	env_cpy->type = ft_strdup(str);
	env_cpy->env = return_path(str);
	return (env_cpy);
}

t_env_cpy	*edit_env(t_env_cpy *env_cpy, t_token *token)
{
	while (env_cpy)
		env_cpy = env_cpy->next;
	while (token)
	{
		env_cpy = fill_token(env_cpy, token->tokens);
		// env_cpy-> = ft_strdup(token->tokens);
		token = token->next;
		// env_cpy = env_cpy->next;
	}
	return (env_cpy);
}

void	ft_export(t_token *token, t_env_cpy *env_cpy)
{
	if (!token->next)
		print_sorted(env_cpy);
	else
	{
		while (token->next)
		{
			env_cpy = edit_env(env_cpy, token);
			token = token->next;
		}
	}
}
