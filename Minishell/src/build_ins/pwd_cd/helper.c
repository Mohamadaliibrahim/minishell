/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 07:45:28 by mohamibr          #+#    #+#             */
/*   Updated: 2024/11/05 22:06:55 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

void	error_statment(t_env_cpy *env_cpy, int x)
{
	env_cpy->last_exit_status = x;
	ft_putstr_fd("cd: invalid option\n", 2);
}
