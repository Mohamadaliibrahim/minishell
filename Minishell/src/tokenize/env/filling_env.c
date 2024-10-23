/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filling_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 08:56:52 by mohamibr          #+#    #+#             */
/*   Updated: 2024/10/23 08:56:53 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

void	fill_new_node(t_env_cpy **new_node, t_env_cpy **head)
{
	(*new_node)->type = ft_strdup("SHLVL");
	(*new_node)->env = ft_strdup("1");
	(*new_node)->equal = true;
	(*new_node)->heredoc_file = NULL;
	(*new_node)->last_exit_status = 0;
	(*new_node)->last_output_fd = -1;
	(*new_node)->last_input_fd = -1;
	(*new_node)->internal_pwd = NULL;
	(*new_node)->internal_oldpwd = NULL;
	(*new_node)->flag = 0;
	(*new_node)->next = (*head);
	(*new_node)->previous = NULL;
}

t_env_cpy	*cpy_env_helper(char *env)
{
	t_env_cpy	*cpy;

	cpy = malloc(sizeof(t_env_cpy));
	if (!cpy)
		return (NULL);
	cpy->env = return_path(env);
	cpy->type = return_type(env);
	cpy->equal = check_for_equal(env);
	cpy->heredoc_file = NULL;
	cpy->last_exit_status = 0;
	cpy->last_output_fd = -1;
	cpy->last_input_fd = -1;
	cpy->next = NULL;
	cpy->previous = NULL;
	cpy->internal_pwd = NULL;
	cpy->internal_oldpwd = NULL;
	cpy->flag = 0;
	return (cpy);
}
