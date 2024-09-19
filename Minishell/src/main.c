/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mustafa-machlouch <mustafa-machlouch@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:07:00 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/09/19 12:00:34 by mustafa-mac      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_env_cpy	*history(void)
{
	char	**dest;
	char	*pwd;

	dest = malloc(sizeof(char *) * 4);
	if (!dest)
		return (NULL);
	pwd = getcwd(NULL, 0);
	dest[0] = ft_strjoin("PWD=", pwd);
	dest[1] = "SHELVL=1";
	dest[2] = "_=/usr/bin/env";
	dest[3] = NULL;
	free(pwd);
	return (cpy_env(dest));
}

int	main(int ac, char **av, char **env)
{
	char		*input;
	t_env_cpy	*env_cpy;

	(void)ac;
	(void)av;
	if (env[0] == NULL)
		env_cpy = history();
	else
		env_cpy = cpy_env(env);
	while (1)
	{
		setup_signal_handlers();
		input = readline("\033[0;36m$>\033[0m ");
		if (input == NULL)
		{
			printf("exit\n");
			exit(0);
		}
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
