/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 20:45:16 by mohamibr          #+#    #+#             */
/*   Updated: 2024/09/21 20:49:00 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	check_token(t_token *token, t_env_cpy   *env_cpy)
{
	t_token	*head

	while (token)
	{
		if (token->token_type == REDIRECT_OUT)
		{}
	}
}