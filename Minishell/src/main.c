/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mustafa-machlouch <mustafa-machlouch@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:07:00 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/09/19 11:32:55 by mustafa-mac      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	main(int ac, char **av, char **env)
{
	char		*input;
	t_env_cpy	*env_cpy;

	(void)ac;
	(void)av;
	env_cpy = cpy_env(env);
	while (1)
	{
		setup_signal_handlers();
		input = readline("\033[0;36m$>\033[0m ");
		if (input == NULL)
			break ;
		if (*input)
			add_history(input);
		check(input, env, env_cpy);
		free(input);
	}
	free_env_list(env_cpy);
	clear_history();
	rl_clear_history();
	rl_free_line_state();
	return (0);
}
