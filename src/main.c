/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:07:00 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/12/26 11:11:39 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	freez(t_env_cpy **env_cpy)
{
	free_env_list(*env_cpy);
	clear_history();
	rl_clear_history();
	rl_free_line_state();
}

int	main(int ac, char **av, char **env)
{
	char		*input;
	t_env_cpy	*env_cpy;

	preparing(ac, av, &env_cpy, env);
	while (1)
	{
		input = readline("\033[0;31mHELL>\033[0m ");
		if (input == NULL)
		{
			ft_printf("exit\n");
			break ;
		}
		if (*input)
			add_history(input);
		if (g_last_signal != 0)
		{
			env_cpy->last_exit_status = 128 + g_last_signal;
			g_last_signal = 0;
		}
		check(input, env_cpy);
		free(input);
	}
	freez(&env_cpy);
	return (0);
}
